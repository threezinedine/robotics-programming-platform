#define __NEW_OVERLOAD_IMPLEMENTATION__
#include "platforms/memory.h"
#include <cstdlib>
#include "platforms/console.h"
#include <cstdio>

namespace
{
    struct MemHeader
    {
        void *ptr;
        const char *file;
        u32 line;
        size_t size;
        MemHeader *next;
        MemHeader *prev;
    };

    MemHeader *Create(void *ptr, const char *file, u32 line, size_t size)
    {
        MemHeader *node = (MemHeader *)malloc(sizeof(MemHeader));
        node->ptr = ptr;
        node->file = file;
        node->line = line;
        node->size = size;
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
            u64 memoryLeaked = GetMemoryAllocated();

            if (memoryLeaked != 0)
            {
                char buffer[512];
                snprintf(buffer, sizeof(buffer), "Memory leak detected: %zu bytes\n", memoryLeaked);

                rpp::print(buffer, rpp::ConsoleColor::YELLOW);
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

// Overload global new operator
void *operator new(size_t size, const char *file, u32 line)
{
    void *ptr = malloc(size);
    if (!ptr)
    {
        throw std::bad_alloc();
    }

    MemHeader *existing = Find(g_memList, ptr);
    if (existing == nullptr)
    {
        MemHeader *node = Create(ptr, file, line, size);
        PushBack(g_memList, node);
    }
    else
    {
        existing->size = size > existing->size ? size : existing->size;
    }

    return ptr;
}

void *operator new[](size_t size, const char *file, u32 line)
{
    void *ptr = malloc(size);

    if (!ptr)
    {
        throw std::bad_alloc();
    }

    MemHeader *existing = Find(g_memList, ptr);
    if (existing == nullptr)
    {
        MemHeader *node = Create(ptr, file, line, size);
        PushBack(g_memList, node);
    }
    else
    {
        existing->size = size > existing->size ? size : existing->size;
    }

    return ptr;
}

// Overload global delete operator
void operator delete(void *ptr) noexcept
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

void operator delete[](void *ptr) noexcept
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

#undef __NEW_OVERLOAD_IMPLEMENTATION__