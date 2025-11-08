#if defined(RPP_DEBUG)

#include <cstdlib>
#include "platforms/console.h"
#include <cstdio>
#include <stdexcept>
#include <cstring>

#include "platforms/memory.h"

#if defined(_MSC_VER)
#include <windows.h>
#include <dbghelp.h>
#define MAX_TRACE_STACK_DEPTH 32
#endif

void operator delete(void *ptr) noexcept
{
    ::rpp::Deallocate(ptr);
}

namespace rpp
{
    static b8 g_memoryTrackingEnabled = FALSE;

    namespace
    {
        struct MemHeader
        {
            void *ptr;
            size_t size;
            const char *file;
            i32 line;
    #if defined(_MSC_VER)
            void *traceStack[MAX_TRACE_STACK_DEPTH];
            u32 traceStackSize;
    #endif
            MemHeader *next;
            MemHeader *prev;
        };

    #if defined(_MSC_VER)
        static void LogStackTrace(MemHeader *node)
        {
            USHORT frames = CaptureStackBackTrace(0, MAX_TRACE_STACK_DEPTH, node->traceStack, nullptr);
            node->traceStackSize = static_cast<u32>(frames);
        }
    #endif

        MemHeader *Create(void *ptr, size_t size, const char *file = nullptr, i32 line = 0)
        {
            MemHeader *node = (MemHeader *)malloc(sizeof(MemHeader));
            node->ptr = ptr;
            node->size = size;
            node->file = file;
            node->line = line;
            node->next = nullptr;
            node->prev = nullptr;

    #if defined(_MSC_VER)
            LogStackTrace(node);
    #endif
            return node;
        }

        void Destroy(MemHeader *node)
        {
            free(node);
        }

        struct MemHeaderList
        {
            MemHeader *head = nullptr;
            MemHeader *tail = nullptr;

            ~MemHeaderList()
            {
                if (g_memoryTrackingEnabled)
                {
                    char buffer[524288];
                    b8 hasLeak = GetMemoryAllocated(buffer, sizeof(buffer));

                    rpp::print(buffer, rpp::ConsoleColor::RED);

    #if RPP_PLATFORM_WINDOWS
                    if (hasLeak)
                    {
                        debugbreak();
                    }
    #endif
                }

                MemHeader *node = head;
                while (node)
                {
                    MemHeader *next = node->next;
                    Destroy(node);
                    node = next;
                }
                head = nullptr;
                tail = nullptr;
            }
        };

        void PushBack(MemHeaderList &list, MemHeader *node)
        {
            if (!list.head)
            {
                list.head = node;
                list.tail = node;
            }
            else
            {
                MemHeader *tail = list.tail;
                tail->next = node;
                node->prev = tail;
                list.tail = node;
            }
        }

        void Remove(MemHeaderList &list, MemHeader *node)
        {
            if (node->prev)
            {
                node->prev->next = node->next;
            }
            else
            {
                list.head = node->next;
            }

            if (node->next)
            {
                node->next->prev = node->prev;
            }

            if (list.tail == node)
            {
                list.tail = node->prev;
            }

            Destroy(node);
        }

        MemHeader *Find(MemHeaderList &list, void *ptr)
        {
            MemHeader *node = list.head;
            while (node)
            {
                if (node->ptr == ptr)
                {
                    return node;
                }
                node = node->next;
            }
            return nullptr;
        }

    } // namespace

    static MemHeaderList g_memList;

    MemoryObject::MemoryObject()
    {
        g_memoryTrackingEnabled = TRUE;
    }

    MemoryObject::~MemoryObject()
    {
        // Destructor will automatically clean up the memory list
    }

    void *Allocate(size_t size, const char *file, i32 line)
    {
        void *ptr = malloc(size);
        if (!ptr)
        {
            throw std::bad_alloc();
        }

        MemHeader *existing = Find(g_memList, ptr);
        if (existing == nullptr)
        {
            MemHeader *node = Create(ptr, size, file, line);
            PushBack(g_memList, node);
        }
        else
        {
            if (existing->size != size)
            {
                debugbreak();
                char message[512];
                snprintf(message, sizeof(message),
                        "Warning: Allocating memory (%s:%d) at an address that is already tracked (%s:%d) with a different size.\n"
                        "Previous size: %zu bytes, New size: %zu bytes.\n",
                        file, line, existing->file != nullptr ? existing->file : "unknown", existing->line, existing->size, size);
                rpp::print(message, rpp::ConsoleColor::YELLOW);
            }
            existing->size = size > existing->size ? size : existing->size;
        }

        return ptr;
    }

    void Deallocate(void *ptr)
    {
        if (!ptr)
        {
            return;
        }

        free(ptr);

        MemHeader *node = Find(g_memList, ptr);
        if (node)
        {
            Remove(g_memList, node);
        }
    }

    u64 GetMemoryAllocated()
    {
        u64 total = 0;
        MemHeader *node = g_memList.head;
        while (node)
        {
            total += node->size;
            node = node->next;
        }
        return total;
    }

    b8 GetMemoryAllocated(char *buffer, size_t bufferSize)
    {
        std::memset(buffer, 0, bufferSize);
        MemHeader *node = g_memList.head;
        u64 total = 0;

        while (node)
        {
            total += node->size;

            snprintf(buffer, bufferSize, "%sLeaked %zu bytes at address %p at %s:%d\n", buffer, node->size / 8, node->ptr, node->file != nullptr ? node->file : "unknown", node->line);

    #if RPP_PLATFORM_WINDOWS
            debugbreak();
    #else 
            print(buffer, rpp::ConsoleColor::YELLOW);
    #endif
            node = node->next;
        }

        snprintf(buffer, bufferSize, "%sTotal memory allocated: %llu bytes\n", buffer, total);

        if (total == 0)
        {
            snprintf(buffer, bufferSize, "No memory leaks detected.\n");
            return FALSE;
        }
        return TRUE;
    }
}

#endif // RPP_PLATFORM_WINDOWS