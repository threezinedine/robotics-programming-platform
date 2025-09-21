#pragma once
#include "platforms/platforms.h"
#include "string.h"

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
         * @brief Constructs a Json object by parsing a JSON-formatted string.
         * @param jsonString The JSON-formatted string to parse.
         */
        Json(const String &jsonString);

        ~Json();

    public:
        /**
         * @brief Serializes the Json object back into a JSON-formatted string.
         * @return A String containing the JSON representation of the object.
         */
        String ToString() const;

    private:
        void *m_data;
    };
} // namespace rpp
