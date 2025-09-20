#pragma once
#include "platforms/platforms.h"
#include "singleton_manager.h"
#include "string.h"
#include "map.h"

namespace rpp
{
    /**
     * The singleton object which records the current allocated memory blocks.
     * This is mainly used for memory leak detection.
     */
    class MemoryManager
    {
        RPP_SINGLETON_DEFINE(MemoryManager);

    public:
        /**
         * @brief Record a memory allocation.
         * @param ptr The pointer to the allocated memory block.
         * @param size The size of the allocated memory block.
         * @param file The source file where the allocation occurred.
         * @param line The line number in the source file where the allocation occurred.
         */
        void Alloc(void *ptr, u32 size, const String &file, u32 line);

        /**
         * @brief Record a memory deallocation.
         * @param ptr The pointer to the deallocated memory block.
         * @param size The size of the deallocated memory block.
         */
        void Free(void *ptr, u32 size);

        /**
         * @brief Get the total allocated memory size.
         * @return The total size of currently allocated memory blocks.
         */
        u32 GetTotalAllocatedMemory() const;

    private:
        /**
         * @brief Private constructor to enforce singleton pattern.
         */
        struct MemoryRecord
        {
            void *ptr;   ///< The pointer to the allocated memory block.
            u32 size;    ///< The size of the allocated memory block.
            String file; ///< The source file where the allocation occurred.
            u32 line;    ///< The line number in the source file where the allocation occurred.
        };

        Map<void *, MemoryRecord> m_memoryRecords; ///< Map of memory records keyed by their addresses. Key is the address of the allocated memory block.
    };

} // namespace rpp

void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;
