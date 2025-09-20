#include "core/format.h"
#include <string>

namespace rpp
{
#define DEFINE_ATOMIIC_FORMAT_FUNC(type)                                \
    template <>                                                         \
    String Format<type>(const String &formatMessage, const type &value) \
    {                                                                   \
        String result = formatMessage;                                  \
        return result.Replace("{}", ToString(value));                   \
    }

    const String ToString(const String &value)
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

#define DEFINE_ATOMIC_FORMAT_AND_TO_STIRING_FUNC(type) \
    template <>                                        \
    const String ToString<type>(const type &value)     \
    {                                                  \
        return String(std::to_string(value).c_str());  \
    }                                                  \
    DEFINE_ATOMIIC_FORMAT_FUNC(type)

    DEFINE_ATOMIC_FORMAT_AND_TO_STIRING_FUNC(u8);
    DEFINE_ATOMIC_FORMAT_AND_TO_STIRING_FUNC(u16);
    DEFINE_ATOMIC_FORMAT_AND_TO_STIRING_FUNC(u32);
    DEFINE_ATOMIC_FORMAT_AND_TO_STIRING_FUNC(u64);

    DEFINE_ATOMIC_FORMAT_AND_TO_STIRING_FUNC(i8);
    DEFINE_ATOMIC_FORMAT_AND_TO_STIRING_FUNC(i16);
    DEFINE_ATOMIC_FORMAT_AND_TO_STIRING_FUNC(i32);
    DEFINE_ATOMIC_FORMAT_AND_TO_STIRING_FUNC(i64);

    DEFINE_ATOMIC_FORMAT_AND_TO_STIRING_FUNC(f32);
    DEFINE_ATOMIC_FORMAT_AND_TO_STIRING_FUNC(f64);
} // namespace rpp
