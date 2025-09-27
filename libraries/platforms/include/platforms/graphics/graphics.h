#pragma once
#include "platforms/common.h"
#include "windows.h"

#if defined(RPP_PLATFORM_WINDOWS) || defined(RPP_PLATFORM_LINUX) || defined(RPP_PLATFORM_MACOS)
#define RPP_GRAPHICS_BACKEND_OPENGL
#else
#error "Graphics backend is not supported on this platform!"
#endif

namespace rpp
{
    class Window;

    /**
     * @brief Graphics Backend interface which contains all needed interfaces which the rendering system needs.
     * This is singleton class.
     * @note If need any new methods, add them here and implement them in the derived class.
     */
    class Graphics
    {
    public:
        /**
         * @brief Initialize the graphics backend, should be called once before using any graphics related features.
         *
         * @return TRUE if the initialization is successful, FALSE otherwise.
         */
        static b8 Init();

        /**
         * @brief Shutdown the graphics backend, must be called once before exiting the application.
         */
        static void Shutdown();

        /**
         * @brief Create a window instance of the graphics backend.
         *
         * @param width The width of the window.
         * @param height The height of the window.
         * @param title The title of the window.
         *
         * @return The window instance, nullptr if the creation failed.
         */
        static Scope<Window> CreateWindow(u32 width, u32 height, const char *title);
    };
} // namespace rpp
