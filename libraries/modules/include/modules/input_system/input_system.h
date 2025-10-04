#pragma once
#include "core/core.h"
#include "defines.h"

namespace rpp
{
    /**
     * The method signature which will be called when a keyboard is pressed or released.
     *
     * @param key The key code of the key event (use RPP_KEY_* defines).
     * @param pressed TRUE if the key is pressed, FALSE if released.
     */
    typedef void (*KeyboardEventCallback)(Key key, b8 pressed);

    /**
     * The method signature which will be called when a mouse button is pressed or released.
     *
     * @param button The mouse button code of the mouse event (use RPP_MOUSE_* defines).
     * @param pressed TRUE if the button is pressed, FALSE if released.
     * @param x The x-coordinate of the mouse cursor when the event occurred.
     * @param y The y-coordinate of the mouse cursor when the event occurred.
     */
    typedef void (*MouseEventCallback)(MouseButton button, b8 pressed, i32 x, i32 y);

    /**
     * The wrapper system for the input devices, including keyboard, mouse. This module can be used for testing by mocking the
     * input events.
     */
    class InputSystem
    {
    public:
        /**
         * Initializes the input system module. This function must be called before using any other functions in this module.
         * It sets up necessary resources and configurations for input handling.
         * @note This function is typically called during the initialization phase of the application.
         */
        static void Initialize();

        /**
         * Shuts down the input system module. This function releases any resources allocated during initialization and
         * cleans up the module state. After calling this function, no other functions in this module should be used.
         * @note This function is typically called during the shutdown phase of the application.
         */
        static void Shutdown();

        /// Normal usage
    public:
        /**
         * Gets the current state of a specific key on the keyboard.
         * @param key The key code to check (use RPP_KEY_* defines).
         * @return The state of the key: 0 = not pressed, 1 = pressed.
         */
        static b8 GetKeyState(Key key);

        /**
         * The previous frame state of a specific key on the keyboard.
         * @param key The key code to check (use RPP_KEY_* defines).
         * @return The previous state of the key: 0 = not pressed, 1 = pressed.
         */
        static b8 GetKeyPreviousState(Key key);

        /**
         * Gets the current state of a specific mouse button.
         * @param button The mouse button code to check (use RPP_MOUSE_* defines).
         * @return The state of the mouse button: 0 = not pressed, 1 = pressed.
         */
        static b8 GetMouseState(MouseButton button);

        /**
         * The previous frame state of a specific mouse button.
         * @param button The mouse button code to check (use RPP_MOUSE_* defines).
         * @return The previous state of the mouse button: 0 = not pressed, 1 = pressed.
         */
        static b8 GetMousePreviousState(MouseButton button);

        /**
         * Gets the current x-coordinate of the mouse cursor.
         * @return The x-coordinate of the mouse cursor.
         */
        static i32 GetMouseX();

        /**
         * Gets the current y-coordinate of the mouse cursor.
         * @return The y-coordinate of the mouse cursor.
         */
        static i32 GetMouseY();

        /**
         * Add the callback function to be invoked on keyboard events.
         * @param callback The function to be called when a keyboard event occurs.
         */
        static void RegisterKeyboardEventCallback(KeyboardEventCallback callback);

        /**
         * Add a callback function to be invoked on mouse button events.
         * @param callback The function to be called when a mouse button event occurs.
         * @note The callback function should match the MouseEventCallback signature.
         */
        static void RegisterMouseEventCallback(MouseEventCallback callback);

        /**
         * Updates the input system state. This function should be called once per frame to process input events
         * and update the state of keys and mouse buttons.
         * @note This function is typically called in the main loop of the application.
         */
        static void Update();

        /// Mocking for testing purpose
    public:
        /**
         * Sets the state of a specific key on the keyboard. This function is intended for testing purposes only.
         * It allows simulating key presses and releases.
         */
        static void SetKeyState(Key key, b8 pressed);

        /**
         * Sets the state of a specific mouse button. This function is intended for testing purposes only.
         * It allows simulating mouse button presses and releases.
         */
        static void SetMouseState(MouseButton button, b8 pressed, i32 x, i32 y);

    private:
        /**
         * Contains all the status information of a key.
         */
        struct KeyState
        {
            Key key;          /// The key code.
            b8 state;         /// Current state.
            b8 previousState; /// Previous state.
        };

        /**
         * Array to hold the status of all keys.
         */
        static KeyState s_keyStatus[RPP_MAX_KEYS];

        /**
         * Contains all the status information of a mouse button.
         */
        struct MouseButtonState
        {
            MouseButton button; /// The mouse button code.
            b8 state;           /// Current state.
            b8 previousState;   /// Previous state.
        };

        /**
         * Array to hold the status of all mouse buttons.
         */
        static MouseButtonState s_mouseButtonStatus[RPP_MAX_MOUSE_BUTTONS];

        static i32 s_mouseX; ///< Current x-coordinate of the mouse cursor.
        static i32 s_mouseY; ///< Current y-coordinate of the mouse cursor.

        static Scope<Array<KeyboardEventCallback>> s_keyboardEventCallbacks; ///< List of registered keyboard event callbacks.
        static Scope<Array<MouseEventCallback>> s_mouseEventCallbacks;       ///< List of registered mouse event callbacks.
    };
} // namespace rpp
