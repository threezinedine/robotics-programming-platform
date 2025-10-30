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
        /**
         * @param width The width of the window.
         * @param height The height of the window.
         * @param title The title of the window.
         * @param pData Pointer to the graphics backend specific data.
         * @param dataSize The size of the graphics backend specific data.
         *
         */
        Window(u32 width, u32 height, const char *title, void *pData = nullptr, u32 dataSize = 0);

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

        /**
         * @brief Get the width of the window.
         * @return The width of the window.
         */
        inline u32 GetWidth() const { return m_width; }

        /**
         * @brief Get the height of the window.
         * @return The height of the window.
         */
        inline u32 GetHeight() const { return m_height; }

        /**
         * Mocking the mouse position setting function.
         * @param xPos The x position of the mouse.
         * @param yPos The y position of the mouse.
         */
        void SetMousePosition(f64 xPos, f64 yPos);

        /**
         * @brief A callback function type for window resize events.
         * @param width The new width of the window.
         * @param height The new height of the window.
         * @param pUserData A pointer to user-defined data passed to the callback.
         */
        typedef void (*WindowResizeCallback)(u32 width, u32 height, void *pUserData);

        inline void SetResizeCallback(WindowResizeCallback callback) { m_resizeCallback = callback; }

        /**
         * @brief A callback function type for mouse move events.
         * @param xPos The new x position of the mouse.
         * @param yPos The new y position of the mouse.
         * @param pUserData A pointer to user-defined data passed to the callback.
         */
        typedef void (*WindowOnMouseMoveCallback)(f64 xPos, f64 yPos, void *pUserData);

        inline void SetOnMouseMoveCallback(WindowOnMouseMoveCallback callback) { m_onMouseMoveCallback = callback; }

    private:
        u32 m_width;         ///< The width of the window.
        u32 m_height;        ///< The height of the window.
        const char *m_title; ///< The title of the window.
        void *m_window;      ///< The native window handle.

        void *m_pData; ///< Pointer to the graphics backend specific data.

        WindowResizeCallback m_resizeCallback;           ///< The callback function for window resize events.
        WindowOnMouseMoveCallback m_onMouseMoveCallback; ///< The callback function for mouse move events.
    };
} // namespace rpp
