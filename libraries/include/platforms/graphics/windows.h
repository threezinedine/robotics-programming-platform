#pragma once
#include "platforms/common.h"
#include "command.h"

namespace rpp
{
    /**
     * All possible physical mouse buttons which are supported by the input system.
     */
    enum MouseButton : i16
    {
        LEFT,
        RIGHT,
        MIDDLE,

        MOUSE_BUTTON_COUNT,
    };

    /**
     * All possible physical keyboard buttons which are supported by the input system.
     */
    enum KeyboardButton : i16
    {
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U,
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_Y,
        KEY_Z,

        KEY_0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_ESCAPE,
        KEY_LCTRL,
        KEY_LSHIFT,
        KEY_LALT,
        KEY_LSYSTEM,
        KEY_RCTRL,
        KEY_RSHIFT,
        KEY_RALT,
        KEY_RSYSTEM,
        KEY_MENU,
        KEY_LBRACKET,
        KEY_RBRACKET,
        KEY_SEMICOLON,
        KEY_COMMA,
        KEY_PERIOD,
        KEY_QUOTE,
        KEY_SLASH,
        KEY_BACKSLASH,
        KEY_TILDE,
        KEY_EQUAL,
        KEY_DASH,
        KEY_SPACE,
        KEY_RETURN,
        KEY_BACKSPACE,
        KEY_TAB,
        KEY_PAGEUP,
        KEY_PAGEDOWN,
        KEY_END,
        KEY_HOME,
        KEY_INSERT,
        KEY_DELETE,
        KEY_ADD,
        KEY_SUBTRACT,
        KEY_MULTIPLY,
        KEY_DIVIDE,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_UP,
        KEY_DOWN,
        KEY_NUMPAD0,
        KEY_NUMPAD1,
        KEY_NUMPAD2,
        KEY_NUMPAD3,
        KEY_NUMPAD4,
        KEY_NUMPAD5,
        KEY_NUMPAD6,
        KEY_NUMPAD7,
        KEY_NUMPAD8,
        KEY_NUMPAD9,
        KEY_F1,
        KEY_F2,
        KEY_F3,
        KEY_F4,
        KEY_F5,
        KEY_F6,
        KEY_F7,
        KEY_F8,
        KEY_F9,
        KEY_F10,
        KEY_F11,
        KEY_F12,

        KEY_ENTER,

        KEYBOARD_BUTTON_COUNT,
    };

    /**
     * All possible action a button can have.
     */
    enum ButtonState : i8
    {
        RELEASED = 0, ///< The button is not pushed.
        PRESSED = 1,  ///< Th button is pushed down.
        REPEAT = 2,   ///< The button is being held down.
    };

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
         * @return Get window mouse position X
         */
        f64 GetMouseX() const;

        /**
         * @return Get window mouse position Y
         */
        f64 GetMouseY() const;

        /**
         * @brief Get the state of the given mouse button.
         * @param button The mouse button to get the state of.
         * @return The state of the given mouse button.
         */
        ButtonState GetMouseButtonState(MouseButton button) const;

        /**
         * @brief Get the state of the given keyboard button.
         * @param button The keyboard button to get the state of.
         * @return The state of the given keyboard button.
         */
        ButtonState GetKeyboardButtonState(KeyboardButton button) const;

        /**
         * @brief Get the title of the window.
         *
         * @return current window title
         */
        const char *GetWindowTitle() const;

        /**
         * Modify the current window title.
         *
         * @param windowTitle
         */
        void SetWindowTitle(const char *windowTitle);

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

        /**
         * @brief A callback function type for window close events.
         * @param pUserData A pointer to user-defined data passed to the callback.
         */
        using WindowOnCloseCallback = std::function<void(void *)>;

        inline void SetOnCloseCallback(WindowOnCloseCallback callback) { m_onCloseCallback = callback; }

    private:
        u32 m_width;         ///< The width of the window.
        u32 m_height;        ///< The height of the window.
        const char *m_title; ///< The title of the window.
        void *m_window;      ///< The native window handle.

        void *m_pData; ///< Pointer to the graphics backend specific data.

        WindowResizeCallback m_resizeCallback;           ///< The callback function for window resize events.
        WindowOnMouseMoveCallback m_onMouseMoveCallback; ///< The callback function for mouse move events.
        WindowOnCloseCallback m_onCloseCallback;         ///< The callback function for window close events.
    };
} // namespace rpp
