#pragma once

namespace rpp
{
    /**
     * @brief All available colors for console printing output.
     */
    enum class ConsoleColor
    {
        WHITE,  ///< Default console color.
        RED,    ///< Red color.
        GREEN,  ///< Green color.
        BLUE,   ///< Blue color.
        YELLOW, ///< Yellow color.
        COUNT,
    };

    /**
     * @brief Print a message to the console with an optional color.
     *
     * @param message The message to print.
     * @param color The color to use for the message (default is WHITE).
     */
    void print(const char *message, ConsoleColor color = ConsoleColor::WHITE);
} // namespace rpp
