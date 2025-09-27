#pragma once
#include "platforms/common.h"
#include "command.h"

namespace rpp
{
    /**
     * @brief The window instance of the graphics backend. Each graphics backend should have its own window instance.
     */
    class Window
    {
    public:
        /*
         * @param width The width of the window.
         * @param height The height of the window.
         * @param title The title of the window.
         *
         */
        Window(u32 width, u32 height, const char *title);

        Window(const Window &other);

        ~Window();

        // Methods of the instance.
    public:
        /**
         * @brief Used for checking whether the window should close or not.
         *
         * @return TRUE if the window should close, FALSE otherwise.
         */
        b8 ShouldWindowClose();

        /**
         * @brief Update and receive the window events.
         */
        void PollEvents();

        /**
         * @brief Execute a graphics command on the window instance.
         *
         * @param command The graphics command to execute.
         *
         * @return TRUE if the command is executed successfully, FALSE otherwise.
         */
        b8 ExecuteCommand(const GraphicsCommandData &command);

    private:
        u32 m_width;         ///< The width of the window.
        u32 m_height;        ///< The height of the window.
        const char *m_title; ///< The title of the window.
        void *m_window;      ///< The native window handle.
    };
} // namespace rpp
