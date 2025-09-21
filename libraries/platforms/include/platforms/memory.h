#pragma once
#include "common.h"

void *operator new(size_t size);
void *operator new[](size_t size);

void *operator new(size_t size, const char *file, i32 line);
void *operator new[](size_t size, const char *file, i32 line);

void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;

void *Allocate(size_t size, const char *file = nullptr, i32 line = 0);
void Deallocate(void *ptr);

#define RPP_NEW(obj) new (__FILE__, __LINE__) obj
#define RPP_MALLOC(size) Allocate(size, __FILE__, __LINE__)
#define RPP_FREE(ptr) Deallocate(ptr)

struct MemoryObject
{
    ~MemoryObject();
};

#define RPP_ENABLE_MEMORY_TRACKING MemoryObject __rpp_memory_object

u64 GetMemoryAllocated();
void GetMemoryAllocated(char *buffer, size_t bufferSize);