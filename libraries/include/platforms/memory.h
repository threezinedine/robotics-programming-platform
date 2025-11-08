#pragma once
#include "common.h"

// ================== Operator overloads for new/delete ==================

#if defined(RPP_DEBUG)
void operator delete(void *ptr) noexcept;

namespace rpp
{
    /**
     * @brief Allocates memory and tracks the allocation if memory tracking is enabled.
     * @param size Size of memory to allocate in bytes.
     * @param file File name where the allocation is made (automatically provided by macro).
     * @param line Line number where the allocation is made (automatically provided by macro).
     *
     * @return Pointer to the allocated memory.
     */
    void *Allocate(size_t size, const char *file = nullptr, i32 line = 0);

    /**
     * @brief Deallocates memory and removes it from tracking if memory tracking is enabled.
     * @param ptr Pointer to the memory to deallocate.
     */
    void Deallocate(void *ptr);

    struct MemoryObject
    {
        MemoryObject();
        ~MemoryObject();
    };

    /**
     * @brief Get the total memory allocated (in bytes).
     * @return Total memory allocated (in bytes).
     */
    u64 GetMemoryAllocated();

    /**
     * @brief Get the memory allocation report.
     * @param buffer Buffer to store the report.
     * @param bufferSize Size of the buffer.
     *
     * @return TRUE if has the memory leaks, FALSE otherwise.
     */
    b8 GetMemoryAllocated(char *buffer, size_t bufferSize);
}

/**
 * @brief Macro to allocate memory with file and line information for tracking.
 *
 * @param obj Object type to allocate.
 * @return Pointer to the allocated object.
 *
 * @example
 * ```cpp
 *    MyClass *obj = RPP_NEW(MyClass()); // Allocates memory for MyClass and tracks it with file and line info.
 * ```
 */
#define RPP_NEW(obj, ...) new (::rpp::Allocate(sizeof(obj), __FILE__, __LINE__)) obj(__VA_ARGS__)

/**
 * @brief Macro to replace an existing object in place using placement new.
 */
#define RPP_NEW_REPLACE(addr, obj) new (addr) obj

/**
 * @brief Macro to deallocate memory and set the pointer to nullptr (which is a good practice to avoid dangling pointers).
 */
#define RPP_DELETE(ptr) delete ptr;         

/**
 * @brief Macro to allocate memory without file and line information. Used for containers' allocations.
 * @param size Size of memory to allocate in bytes.
 *
 * @return Pointer to the allocated memory.
 *
 * @example
 * ```cpp
 *   Test *mem = (Test *)RPP_MALLOC(1024); // Allocates 1024 bytes of memory and tracks it with file and line info.
 * ```
 */
#define RPP_MALLOC(size) ::rpp::Allocate(size, __FILE__, __LINE__)

// #define RPP_NEW_ARRAY(type, count) new (Allocate(sizeof(type) * (count), __FILE__, __LINE__)) type[count]

/**
 * @brief Macro to deallocate memory.
 *
 * @param ptr Pointer to the memory to deallocate.
 *
 * @example
 * ```cpp
 *   RPP_FREE(mem); // Deallocates memory pointed by mem and removes it from tracking.
 * ```
 */
#define RPP_FREE(ptr) ::rpp::Deallocate(ptr)

#define RPP_NEW_ARRAY(ptr, type, count)                   \
    do                                                    \
    {                                                     \
        ptr = (type *)RPP_MALLOC(sizeof(type) * (count)); \
        for (u32 index = 0u; index < (count); ++index)    \
        {                                                 \
            new (&((type *)ptr)[index]) type();           \
        }                                                 \
    } while (0)

#define RPP_DELETE_ARRAY(ptr, type, count)             \
    do                                                 \
    {                                                  \
        if (ptr != nullptr)                            \
        {                                              \
            continue;                                  \
        }                                              \
        for (u32 index = 0u; index < (count); ++index) \
        {                                              \
            ((type *)ptr)[index].~type();              \
        }                                              \
        RPP_FREE(ptr);                                 \
    } while (0)

// Used at main.cpp to enable memory tracking.
#define RPP_ENABLE_MEMORY_TRACKING ::rpp::MemoryObject __rpp_memory_object

#else
#define RPP_NEW(obj, ...) new obj(__VA_ARGS__)

#define RPP_NEW_REPLACE(addr, obj) new (addr) obj
#define RPP_DELETE(ptr) delete (ptr)

#define RPP_MALLOC(size) malloc(size)
#define RPP_FREE(ptr) free(ptr)

#define RPP_NEW_ARRAY(ptr, type, count) do { ptr = new type[count]; } while (0)
#define RPP_DELETE_ARRAY(ptr, type, count) delete[] (ptr)

#define RPP_ENABLE_MEMORY_TRACKING
#endif