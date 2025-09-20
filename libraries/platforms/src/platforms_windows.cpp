#include "platforms/platforms.h"
#include <cstdio>
#include <stdexcept>

#if RPP_PLATFORM_WINDOWS

namespace rpp
{
    void print(const char *message, ConsoleColor color)
    {
        switch (color)
        {
        case ConsoleColor::WHITE:
            std::printf("\033[0m%s\033[0m", message); // Reset to default color
            break;
        case ConsoleColor::RED:
            std::printf("\033[31m%s\033[0m", message); // Red color
            break;
        case ConsoleColor::GREEN:
            std::printf("\033[32m%s\033[0m", message); // Green color
            break;
        case ConsoleColor::BLUE:
            std::printf("\033[34m%s\033[0m", message); // Blue color
            break;
        case ConsoleColor::YELLOW:
            std::printf("\033[33m%s\033[0m", message); // Yellow color
            break;
        default:
            throw std::runtime_error("Unsupported console color");
        }
    }
} // namespace rpp

#endif // RPP_PLATFORM_WINDOWS