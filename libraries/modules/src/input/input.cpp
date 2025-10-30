#include "modules/input/input_internal.h"
#include "modules/renderer/renderer.h"

/**
 * If the current mouse position and the previous position -> reset the previous mouse position
 */
#define MAX_MOUSE_DELTA 10

namespace rpp
{
    f64 InputSystem::m_mouseX = 0.0;
    f64 InputSystem::m_mouseY = 0.0;

    f64 InputSystem::m_previousMouseX = 0.0;
    f64 InputSystem::m_previousMouseY = 0.0;

    void InputSystem::Initialize()
    {
        // Initialization code for the input system can be added here.
    }

    void InputSystem::Shutdown()
    {
        // Cleanup code for the input system can be added here.
    }

    void InputSystem::Update(f32 delta)
    {
        // Update mouse position or other input states here.
        // For demonstration, we'll just keep the mouse position unchanged.
    }

    b8 InputSystem::MoveMouseTo(f64 x, f64 y)
    {
        return TRUE;
    }

    void InputSystem::OnMouseMoveCallback(f64 xPos, f64 yPos, void *pUserData)
    {
#if defined(RPP_USE_TEST)
        m_mouseX = 100;
        m_mouseY = 200;

        Renderer::GetWindow()->SetMousePosition(InputSystem::GetMouseX(), InputSystem::GetMouseY());
#else
        if (abs(xPos - m_mouseX) > MAX_MOUSE_DELTA || abs(yPos - m_mouseY) > MAX_MOUSE_DELTA)
        {
            m_previousMouseX = xPos;
            m_previousMouseY = yPos;
        }
        else
        {
            m_previousMouseX = m_mouseX;
            m_previousMouseY = m_mouseY;
        }

        m_mouseX = xPos;
        m_mouseY = yPos;

#endif
    }
} // namespace rpp
