#include "core/format.h"
#include <string>

namespace rpp
{
    String Format(const String &formatMessage)
    {
        return formatMessage;
    }

#define DEFINE_ATOMIIC_FORMAT_FUNC(type)                                \
    template <>                                                         \
    String Format<type>(const String &formatMessage, const type &value) \
    {                                                                   \
        String result = formatMessage;                                  \
        return result.Replace("{}", ToString(value));                   \
    }

    template <>
    const String ToString<String>(const String &value)
    {
        return value;
    }

    // DEFINE_ATOMIIC_FORMAT_FUNC(String);

    template <>
    String Format<String>(const String &formatMessage, const String &value)
    {
        String result = formatMessage;
        return result.Replace("{}", ToString(value));
    }

#define DEFINE_ATOMIC_FORMAT_AND_TO_STRING_FUNC(type) \
    template <>                                       \
    const String ToString<type>(const type &value)    \
    {                                                 \
        return String(std::to_string(value).c_str()); \
    }                                                 \
    DEFINE_ATOMIIC_FORMAT_FUNC(type)

    DEFINE_ATOMIC_FORMAT_AND_TO_STRING_FUNC(u8);
    DEFINE_ATOMIC_FORMAT_AND_TO_STRING_FUNC(u16);
    DEFINE_ATOMIC_FORMAT_AND_TO_STRING_FUNC(u32);
    DEFINE_ATOMIC_FORMAT_AND_TO_STRING_FUNC(u64);

    DEFINE_ATOMIC_FORMAT_AND_TO_STRING_FUNC(i8);
    DEFINE_ATOMIC_FORMAT_AND_TO_STRING_FUNC(i16);
    DEFINE_ATOMIC_FORMAT_AND_TO_STRING_FUNC(i32);
    DEFINE_ATOMIC_FORMAT_AND_TO_STRING_FUNC(i64);

    template <>
    const String ToString<f32>(const f32 &value)
    {
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "%.2f", value);
        return String(buffer);
    }

    DEFINE_ATOMIIC_FORMAT_FUNC(f32);

    template <>
    const String ToString<f64>(const f64 &value)
    {
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "%.2lf", value);
        return String(buffer);
    }
    DEFINE_ATOMIIC_FORMAT_FUNC(f64);

    template <>
    const String ToString<b8>(const b8 &value)
    {
        return value ? "true" : "false";
    }

    DEFINE_ATOMIIC_FORMAT_FUNC(b8);
} // namespace rpp
