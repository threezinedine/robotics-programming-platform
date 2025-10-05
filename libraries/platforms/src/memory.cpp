#if defined(RPP_PLATFORM_WINDOWS) && defined(RPP_DEBUG)

#include <cstdlib>
#include "platforms/console.h"
#include <cstdio>
#include <stdexcept>
#include <cstring>

#include "platforms/memory.h"

static b8 g_memoryTrackingEnabled = FALSE;
namespace
{
    struct MemHeader
    {
        void *ptr;
        size_t size;
        const char *file;
        i32 line;
        MemHeader *next;
        MemHeader *prev;
    };

    MemHeader *Create(void *ptr, size_t size, const char *file = nullptr, i32 line = 0)
    {
        MemHeader *node = (MemHeader *)malloc(sizeof(MemHeader));
        node->ptr = ptr;
        node->size = size;
        node->file = file;
        node->line = line;
        node->next = nullptr;
        node->prev = nullptr;
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
                GetMemoryAllocated(buffer, sizeof(buffer));

                rpp::print(buffer, rpp::ConsoleColor::RED);
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
            rpp::print("Warning: Allocating memory at an address that is already tracked with a different size.\n", rpp::ConsoleColor::YELLOW);
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

// Overload global new operator
void *operator new(size_t size)
{
    return Allocate(size);
}

void *operator new[](size_t size)
{
    return Allocate(size);
}

void *operator new(size_t size, const char *file, i32 line)
{
    return Allocate(size, file, line);
}

void *operator new[](size_t size, const char *file, i32 line)
{
    return Allocate(size, file, line);
}

// Overload global delete operator
void operator delete(void *ptr) noexcept
{
    Deallocate(ptr);
}

void operator delete[](void *ptr) noexcept
{
    Deallocate(ptr);
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

void GetMemoryAllocated(char *buffer, size_t bufferSize)
{
    std::memset(buffer, 0, bufferSize);
    MemHeader *node = g_memList.head;
    u64 total = 0;

    while (node)
    {
        total += node->size;

        snprintf(buffer, bufferSize, "%sLeaked %zu bytes at address %p at %s:%d\n", buffer, node->size / 8, node->ptr, node->file != nullptr ? node->file : "unknown", node->line);

        node = node->next;
    }

    snprintf(buffer, bufferSize, "%sTotal memory allocated: %zu bytes\n", buffer, total / 8);

    if (total == 0)
    {
        snprintf(buffer, bufferSize, "No memory leaks detected.\n");
    }
}

#endif // RPP_PLATFORM_WINDOWS