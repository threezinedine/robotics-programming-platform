#pragma once
#include "logging.h"

#if defined(RPP_PLATFORM_WINDOWS)
#include <cassert>
#define debugbreak() __debugbreak()
#endif

namespace rpp
{
#if defined(RPP_DEBUG)
#define RPP_ASSERT(condition)                           \
    if (!(condition))                                   \
    {                                                   \
        RPP_LOG_ERROR("Assertion failed: " #condition); \
        debugbreak();                                   \
    }

#define RPP_ASSERT_MSG(condition, message) \
    if (!(condition))                      \
    {                                      \
        RPP_LOG_ERROR(message);            \
        debugbreak();                      \
    }
#else
#define RPP_ASSERT(condition)
#define RPP_ASSERT_MSG(condition, message)
#endif
} // namespace rpp
