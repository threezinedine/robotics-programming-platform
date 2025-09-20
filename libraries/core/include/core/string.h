#pragma once
#include "platforms/platforms.h"

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

    private:
        char *m_data;
    };
} // namespace rpp