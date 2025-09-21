#pragma once
#include "platforms/platforms.h"
#include "string.h"
#include "array.h"

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
         * @brief Sets the value for the specified key in the JSON object. If the key already exists, its value is updated; otherwise, a new key-value pair is added.
         * @param key The key for which to set the value.
         * @param value The value to set for the specified key.
         * @note This method supports values of type String and various integer types (u32, u16, u8, i32, i16, i8).
         */
        template <typename T>
        void Set(const String &key, const T &value);

        /**
         * @brief Checks if the JSON object is empty (i.e., contains no key-value pairs).
         * @return TRUE if the JSON object is empty, FALSE otherwise.
         */
        b8 Empty() const;

        /**
         * @brief Query the specified key to determine if it is an array.
         * @param key The key to check.
         * @return The value associated with the specified key, value will be ignored if the key is not found or if the type does not match.
         *      The result is always an array.
         */
        template <typename T>
        Array<T> GetArray(const String &key) const;

    private:
        void *m_data;
    };
} // namespace rpp
