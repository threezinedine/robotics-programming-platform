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
        };

    public:
        /**
         * Starting the input system. Be called once at the beginning of the program.
         *
         * @note Must be called before initializing `Renderer`.
         */
        static void
        Initialize();

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
         *
         * inputSystem.Update(deltaTime);
         *
         * while (!inputSystem.MoveMouseTo(400.0, 300.0)); // Keep calling Update until the mouse reaches the target position
         * ```
         */
        static b8 MoveMouseTo(f64 x, f64 y) RPP_E2E_BINDING;

        /**
         * The callback which is assigned into the windows to update the current state of the input system.
         * @param xPos New mouse X position in screen coordinates.
         * @param yPos New mouse Y position in screen coordinates.
         * @param pUserData Pointer to user data (not used here).
         */
        static void OnMouseMoveCallback(f64 xPos, f64 yPos, void *pUserData);

    private:
#if 0
        static f64 m_mouseX;
        static f64 m_mouseY;

        static f64 m_previousMouseX;
        static f64 m_previousMouseY;
#else
        static RendererInputData s_inputSystemsData[MAX_INPUT_SYSTEMS]; ///< TODO: Use storage later
#endif
    };
} // namespace rpp
