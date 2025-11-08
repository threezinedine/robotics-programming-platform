#pragma once
#include "platforms/platforms.h"
#include "string.h"
#include "containers/array.h"

namespace rpp
{
    /**
     * @brief A wrapper for JSON data structure, providing parsing and serialization functionalities.
     *      This class is designed to handle JSON formatted strings, allowing for easy manipulation and access to JSON data.
     */
    class Json
    {
    public:
        /**
         * Default constructor. Initializes an empty JSON object.
         */
        Json();

        /**
         * @brief Constructs a Json object by parsing a JSON-formatted string.
         * @param jsonString The JSON-formatted string to parse.
         */
        Json(const String &jsonString);

        /**
         * @brief Copy constructor. Creates a deep copy of another Json object.
         */
        Json(const Json &other);

        Json(Json &&other) noexcept;

        ~Json();

    public:
        /**
         * @brief Serializes the Json object back into a JSON-formatted string.
         * @return A String containing the JSON representation of the object.
         */
        String ToString() const;

        /**
         * @brief Retrieves the value associated with the specified key, returning a default value if the key is not found or if the type does not match.
         * @tparam T The expected type of the value to retrieve. Supported types include String
         *     and various integer types (u32, u16, u8, i32, i16, i8).
         * @param key The key whose associated value is to be retrieved.
         * @param defaultValue The value to return if the key is not found or if the type does not match. Defaults to the default-constructed value of type T.
         * @return The value associated with the specified key, or the default value if the key
         */
        template <typename T>
        T Get(const String &key, const T defaultValue = T()) const;

        /**
         * @brief The overloaded Get method for accessing elements in a JSON array by index (only support for array type).
         * @tparam T The expected type of the value to retrieve from the array. Supported types include String
         *     and various integer types (u32, u16, u8, i32, i16, i8).
         * @param index The index of the element in the JSON array to retrieve.
         *
         * @note If the JSON object is not an array, always return the default value.
         */
        template <typename T>
        T Get(i32 index, const T defaultValue = T()) const;

        /**
         * @brief Sets the value for the specified key in the JSON object. If the key already exists, its value is updated; otherwise, a new key-value pair is added.
         * @param key The key for which to set the value.
         * @param value The value to set for the specified key.
         * @note This method supports values of type String and various integer types (u32, u16, u8, i32, i16, i8).
         */
        template <typename T>
        void Set(const String &key, const T &value);

        /**
         * @brief The overloaded Set method for setting elements in a JSON array by index (only support for array type).
         * @tparam T The type of the value to set in the array. Supported types include String
         *     and various integer types (u32, u16, u8, i32, i16, i8).
         * @param index The index of the element in the JSON array to set.
         * @param value The value to set at the specified index.
         *
         * @note If the JSON object is not an array, this operation will be ignored.
         */
        template <typename T>
        void Set(i32 index, const T &value);

        /**
         * @brief Appends a value to the end of the JSON array. If the JSON object is not an array, this operation will be ignored.
         * @tparam T The type of the value to append to the array. Supported types include String
         *     and various integer types (u32, u16, u8, i32, i16, i8).
         * @param value The value to append to the JSON array.
         */
        template <typename T>
        void Append(const T &value);

        /**
         * @brief Checks if the JSON object is empty (i.e., contains no key-value pairs).
         * @return TRUE if the JSON object is empty, FALSE otherwise.
         */
        b8 Empty() const;

        /**
         * @brief Checks if the JSON object represents an array.
         * @return TRUE if the JSON object is an array, FALSE otherwise.
         */
        b8 IsArray() const;

        /**
         * @brief Use only for array object, get the size of the array. If the object is not an array, return 0.
         *
         * @return The size of the array, or 0 if the object is not an array.
         */
        u32 Size() const;

    private:
        void *m_data;
    };

    /**
     * @brief Converts a string representation to a value of the specified type. Must write the specialization for the needed type.
     *
     * @param str The string to convert.
     * @param outValue The output value to store the converted result.
     *
     * @return TRUE if the conversion was successful, FALSE otherwise.
     *
     * @note Autogen will generate the specializations for the following types: using `RPP_JSON` and `RPP_JSON_KEY` macros.
     */
    template <typename T>
    T FromString(const String &str);
} // namespace rpp
