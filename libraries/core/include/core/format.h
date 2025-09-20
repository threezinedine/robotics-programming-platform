#pragma once
#include "platforms/platforms.h"
#include "string.h"

namespace rpp
{
    String Format(const String &formatMessage);

    template <typename T>
    String Format(const String &formatMessage, const T &value);

    template <typename T, typename... Args>
    String Format(const String &formatMessage, const T &value, const Args &...args)
    {
        String result = formatMessage;
        result = result.Replace("{}", ToString(value));
        return Format(result, args...);
    }
} // namespace rpp
