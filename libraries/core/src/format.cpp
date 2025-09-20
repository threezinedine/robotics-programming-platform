#include "core/format.h"
#include <string>

namespace rpp
{
    String Format(const String &formatMessage)
    {
        return formatMessage;
    }

    template <>
    const String ToString<String>(const String &value)
    {
        return value;
    }

#define DEFINE_TO_STRING(type)                        \
    template <>                                       \
    const String ToString<type>(const type &value)    \
    {                                                 \
        return String(std::to_string(value).c_str()); \
    }

    DEFINE_TO_STRING(u8);
    DEFINE_TO_STRING(u16);
    DEFINE_TO_STRING(u32);
    DEFINE_TO_STRING(u64);

    DEFINE_TO_STRING(i8);
    DEFINE_TO_STRING(i16);
    DEFINE_TO_STRING(i32);
    DEFINE_TO_STRING(i64);

    template <>
    const String ToString<f32>(const f32 &value)
    {
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "%.2f", value);
        return String(buffer);
    }

    template <>
    const String ToString<f64>(const f64 &value)
    {
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "%.2lf", value);
        return String(buffer);
    }

    template <>
    const String ToString<b8>(const b8 &value)
    {
        return value ? "true" : "false";
    }
} // namespace rpp
