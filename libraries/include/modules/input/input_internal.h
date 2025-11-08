#pragma once
#include "core/core.h"

#define MAX_INPUT_SYSTEMS 32

namespace rpp
{
    /**
     * The input system is used for working with the current input states of the current active renderer.
     */
    class RPP_E2E_BINDING InputSystem
    {
    public:
        /**
         * Stored the needed information for a renderer's input system.
         */
        struct RendererInputData
        {
            f64 m_mouseX;
            f64 m_mouseY;

            f64 m_previousMouseX;
            f64 m_previousMouseY;

            ButtonState mouseButtonStates[MouseButton::MOUSE_BUTTON_COUNT]; ///< Left, Right, Middle
            ButtonState mousePreviousButtonStates[MouseButton::MOUSE_BUTTON_COUNT];

            ButtonState keyboardButton[KeyboardButton::KEYBOARD_BUTTON_COUNT];
            ButtonState keyboardPreviousButton[KeyboardButton::KEYBOARD_BUTTON_COUNT];
        };

    public:
        /**
         * Starting the input system. Be called once at the beginning of the program.
         *
         * @note Must be called before initializing `Renderer`.
         */
        static void Initialize();

        /**
         * Release the input system. Be called once at the end of the program.
         */
        static void Shutdown();

        /**
         * Update the input system. Be called once per frame for each active renderer.
         * @param delta Time elapsed since last frame in seconds.
         */
        static void Update(f32 delta);

        /**
         * Get the mouse X position in screen coordinates.
         * @return Mouse X position in screen coordinates.
         */
        static f64 GetMouseX();

        /**
         * Get the mouse Y position in screen coordinates.
         * @return Mouse Y position in screen coordinates.
         */
        static f64 GetMouseY();

        /**
         * @return The difference between current mouse position X with the previous frame
         */
        static f64 GetDeltaX();

        /**
         * @return The difference between current mouse position Y with the previous frame
         */
        static f64 GetDeltaY();

        /**
         * Check if the given mouse button is currently pressed down.
         * @param button The mouse button to check.
         * @return true if the button is pressed down, false otherwise.
         */
        static b8 IsMouseButtonDown(MouseButton button);

        /**
         * Check if the given keyboard button is currently pressed down.
         * @param button The keyboard button to check.
         * @return true if the button is pressed down, false otherwise.
         */
        static b8 IsKeyboardButtonDown(KeyboardButton button);

        /**
         * @note used in testing only, if not in test mode, this method does nothing.
         * Updating the current mouse position to the given coordinates, this process will take time.
         *
         * @param x New mouse X position in screen coordinates.
         * @param y New mouse Y position in screen coordinates.
         * @return true if the mouse position reached the target position, false otherwise.
         *
         * @example
         * ```cpp
         * // Move mouse to position (400, 300)
         * while (!InputSystem::MoveMouseTo(400.0, 300.0)); // Keep calling Update until the mouse reaches the target position
         * ```
         */
        static b8 MoveMouseTo(f64 x, f64 y) RPP_E2E_BINDING;

        /**
         * @note used in testing only, if not in test mode, this method does nothing.
         * Simulate a mouse click on the given button.
         *
         * @param button The mouse button to click.
         * @return true if the click was completed successfully, false otherwise.
         *
         * @example
         * ```cpp
         * // Simulate a left mouse button click
         * while (!InputSystem::MoveMouseTo(400.0, 300.0)); // Move mouse to position (400, 300)
         * while (!InputSystem::ClickMouse(MouseButton::LEFT)); // Click the left mouse button
         */
        static b8 ClickMouse(i32 button) RPP_E2E_BINDING;

        /**
         * @note used in testing only, if not in test mode, this method does nothing.
         * Simulate a keyboard button press.
         *
         * @param button The keyboard button to press.
         * @return true if the button press was completed successfully, false otherwise.
         *
         * @example
         * ```cpp
         * // Simulate pressing the 'A' key
         * while (!InputSystem::PressButton(KeyboardButton::KEY_A));
         * ```
         */
        static b8 Press(KeyboardButton button);

        /**
         * The callback which is assigned into the windows to update the current state of the input system.
         * @param xPos New mouse X position in screen coordinates.
         * @param yPos New mouse Y position in screen coordinates.
         * @param pUserData Pointer to user data (not used here).
         */
        static void OnMouseMoveCallback(f64 xPos, f64 yPos, void *pUserData);

    private:
        static RendererInputData s_inputSystemsData[MAX_INPUT_SYSTEMS]; ///< TODO: Use storage later

        static b8 s_startEvent;      ///< Checking whether the event is started, primarily be used for click simulation.
        static HighResTimer s_timer; ///< Timer used for input system operations.
    };
} // namespace rpp
