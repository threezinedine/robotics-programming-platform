#pragma once
#include "common.h"

void *operator new(size_t size, const char *file, u32 line);
void *operator new[](size_t size, const char *file, u32 line);

void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;

#ifndef __NEW_OVERLOAD_IMPLEMENTATION__
#define new new (__FILE__, __LINE__)
#endif

u64 GetMemoryAllocated();