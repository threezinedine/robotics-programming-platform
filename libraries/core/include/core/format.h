#pragma once
#include "platforms/platforms.h"
#include "string.h"

namespace rpp
{
    /**
     * @brief Converts a format string with placeholders and a variable number of arguments into a formatted string.
     *
     * @return A formatted String.
     */
    String Format(const String &formatMessage);

    /**
     * @brief Converts a value of any type to its string representation.
     * @tparam T The type of the value to convert.
     * @param value The value to convert to a string.
     *
     * @return A String representing the value.
     */
    template <typename T>
    String Format(const String &formatMessage, const T &value)
    {
        String result = formatMessage;
        return result.Replace("{}", ToString<T>(value));
    }

    /**
     * Formatting function that supports multiple arguments.
     * It replaces each occurrence of "{}" in the formatMessage with the string representation of the corresponding argument.
     * @tparam T The type of the first argument.
     * @tparam Args The types of the remaining arguments.
     * @param formatMessage The format string containing "{}" placeholders.
     * @param value The first argument to format.
     * @param args The remaining arguments to format.
     *
     * @return A formatted String with all placeholders replaced by the corresponding argument values.
     *
     * @note If a new class type is needed to be formatted, you must provide a specialization of the ToString function for that type.
     * Example:
     * ```cpp
     * class MyClass { ... };
     * template <>
     * const String ToString<MyClass>(const MyClass &value);
     *
     * ```
     *
     */
    template <typename T, typename... Args>
    String Format(const String &formatMessage, const T &value, const Args &...args)
    {
        String result = formatMessage;
        result = result.Replace("{}", ToString(value));
        return Format(result, args...);
    }
} // namespace rpp
