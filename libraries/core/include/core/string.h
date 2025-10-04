#pragma once
#include "platforms/platforms.h"
#include "containers/array.h"

namespace rpp
{
    /**
     * @brief A simple wrapper around C-style strings.
     */
    class String
    {
    public:
        /**
         * @brief Default constructor. Initializes an empty string.
         */
        String();

        /**
         * @brief Constructs a String from a C-style string.
         *
         * @param str The C-style string to initialize from.
         */
        String(const char *str);

        /**
         * @brief Copy constructor.
         */
        String(const String &other);

        ~String();

    public:
        /**
         * @brief Returns the length of the string.
         *
         * @return The length of the string.
         */
        u32 Length() const;

        /**
         * @brief Returns the underlying C-style string.
         */
        const char *CStr() const;

        // operator
    public:
        /**
         * @brief Accesses the character at the specified index. Throws std::out_of_range if index is invalid.
         */
        char operator[](u32 index) const;

        /**
         * @brief Assignment operator. Performs a deep copy of the other string.
         */
        void operator=(const String &other);

        /**
         * @brief Concatenation operator. Returns a new String that is the concatenation of this string and another.
         */
        String operator+(const String &other);

        /**
         * @brief Concatenation operator with C-style string. Returns a new String that is the concatenation of this string and the C-style string.
         */
        void operator+=(const String &other);

        /**
         * @brief Equality operator. Compares this string with another for equality.
         */
        b8 operator==(const String &other) const;

        /**
         * @brief Inequality operator. Compares this string with another for inequality.
         */
        inline b8 operator!=(const String &other) const { return !(*this == other); }

    public:
        /**
         * @brief Finds the first occurrence of a substring within the string, starting from an optional index.
         *
         * @param substr The substring to search for.
         * @param startIndex The index to start the search from. Default is 0.
         *
         * @return The index of the first occurrence of the substring, or -1 if not found.
         */
        i32 Find(const String &substr, u32 startIndex = 0) const;

        /**
         * @brief Extracts a substring from the string.
         * @param startIndex The starting index of the substring.
         * @param length The length of the substring to extract. If -1, extracts to the end of the string.
         *
         * @note If the startIndex is out of range, an empty string is returned. If the length exceeds the bounds of the string, it is adjusted to fit within the string.
         *
         * @return A new String object containing the extracted substring.
         */
        String SubString(u32 startIndex, i32 length = -1) const;

        /**
         * @brief Replaces occurrences of a substring with another substring.
         * @param oldSubstr The substring to be replaced.
         * @param newSubstr The substring to replace with.
         * @param replaceAll If TRUE, replaces all occurrences; if FALSE, replaces only the first occurrence. Default is FALSE.
         *
         * @return TRUE if at least one replacement was made, FALSE otherwise.
         */
        String Replace(const String &oldSubstr, const String &newSubstr, b8 replaceAll = FALSE);

        /**
         * @brief Concatenates this string with another string and returns the result as a new String object.
         *
         * @param other The string to concatenate with.
         * @return A new String object containing the concatenated result.
         */
        String Concat(const String &other) const;

        /**
         * Split the string into an array of substrings based on a delimiter.
         * @param delimiter The delimiter string to split by.
         * @param outParts The array to store the resulting substrings.
         */
        void Split(Array<String> &outParts, const String &delimiter) const;

        /**
         * @brief Checks if the string ends with the specified suffix.
         * @param suffix The suffix to check.
         * @return TRUE if the string ends with the suffix, FALSE otherwise.
         */
        b8 EndsWith(const String &suffix) const;

        /**
         * @brief Converts the string to lowercase.
         * @return A new String object containing the lowercase version of the string.
         * @note This function does not modify the original string.
         */
        String ToLowerCase() const;

        /**
         * @brief Joins an array of strings into a single string with a specified delimiter.
         * @param parts The array of strings to join.
         * @param delimiter The delimiter to insert between each string.
         * @return A new String object containing the joined result.
         */
        static String Join(const Array<String> &parts, const String &delimiter);

    private:
        char *m_data;
    };

    /**
     * @brief Converts a value of any type to its string representation.
     * @tparam T The type of the value to convert.
     * @param value The value to convert to a string.
     * @return A String representing the value.
     */
    template <typename T>
    const String ToString(const T &value);
} // namespace rpp