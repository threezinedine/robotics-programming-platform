#pragma once
#include <cassert>

typedef unsigned char u8;       ///< 8-bit unsigned integer.
typedef unsigned short u16;     ///< 16-bit unsigned integer.
typedef unsigned int u32;       ///< 32-bit unsigned integer.
typedef unsigned long long u64; ///< 64-bit unsigned integer.

typedef char i8;       ///< 8-bit signed integer.
typedef short i16;     ///< 16-bit signed integer.
typedef int i32;       ///< 32-bit signed integer.
typedef long long i64; ///< 64-bit signed integer.

typedef float f32;  ///< 32-bit floating point.
typedef double f64; ///< 64-bit floating point.

typedef bool b8;  ///< 8-bit boolean.
typedef bool b32; ///< 32-bit boolean.

// ----------------- VALIDATION -----------------
static_assert(sizeof(u8) == 1, "u8 must be 1 byte");
static_assert(sizeof(u16) == 2, "u16 must be 2 bytes");
static_assert(sizeof(u32) == 4, "u32 must be 4 bytes");
static_assert(sizeof(u64) == 8, "u64 must be 8 bytes");

static_assert(sizeof(i8) == 1, "i8 must be 1 byte");
static_assert(sizeof(i16) == 2, "i16 must be 2 bytes");
static_assert(sizeof(i32) == 4, "i32 must be 4 bytes");
static_assert(sizeof(i64) == 8, "i64 must be 8 bytes");

static_assert(sizeof(f32) == 4, "f32 must be 4 bytes");
static_assert(sizeof(f64) == 8, "f64 must be 8 bytes");

#define TRUE b8(true)   ///< Boolean true.
#define FALSE b8(false) ///< Boolean false.