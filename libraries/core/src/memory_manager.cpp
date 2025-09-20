#include "core/memory_manager.h"
#include "core/logging.h"

namespace rpp
{
    RPP_SINGLETON_IMPLEMENT(MemoryManager);

    MemoryManager::MemoryManager()
    {
    }

    MemoryManager::~MemoryManager()
    {
        if (m_memoryRecords.empty())
        {
            RPP_LOG_DEBUG("No memory leaks detected.");
        }
        else
        {
            RPP_LOG_WARNING("Memory leaks detected: {} bytes", GetTotalAllocatedMemory());
        }
    }

    void MemoryManager::Alloc(void *ptr, u32 size, const String &file, u32 line)
    {
        if (m_memoryRecords.find(ptr) != m_memoryRecords.end())
        {
            RPP_LOG_WARNING("Memory block at address is already allocated. Possible double allocation detected.");
            return;
        }

        MemoryRecord record;
        record.ptr = ptr;
        record.size = size;
        record.file = file;
        record.line = line;

        m_memoryRecords[ptr] = record;
    }

    void MemoryManager::Free(void *ptr, u32 size)
    {
        auto it = m_memoryRecords.find(ptr);
        if (it == m_memoryRecords.end())
        {
            RPP_LOG_WARNING("Free non-tracked memory block. Possible double free or invalid free detected.");
            return;
        }

        if (it->second.size != size)
        {
            RPP_LOG_WARNING("Free non-tracked memory block. Possible double free or invalid free detected.");
        }

        m_memoryRecords.erase(it);
    }

    u32 MemoryManager::GetTotalAllocatedMemory() const
    {
        u32 totalSize = 0;
        for (const auto &pair : m_memoryRecords)
        {
            totalSize += pair.second.size;
        }
        return totalSize;
    }

} // namespace rpp

void *operator new(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
    {
        throw std::bad_alloc();
    }
    rpp::MemoryManager::GetInstance()->Alloc(ptr, static_cast<u32>(size), "Unknown", 0);
    return ptr;
}

void *operator new[](size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
    {
        throw std::bad_alloc();
    }
    rpp::MemoryManager::GetInstance()->Alloc(ptr, static_cast<u32>(size), "Unknown", 0);
    return ptr;
}

void operator delete(void *ptr) noexcept
{
    if (ptr)
    {
        rpp::MemoryManager::GetInstance()->Free(ptr, 0);
        free(ptr);
    }
}

void operator delete[](void *ptr) noexcept
{
    if (ptr)
    {
        rpp::MemoryManager::GetInstance()->Free(ptr, 0);
        free(ptr);
    }
}