#include "modules/input/input_internal.h"
#include "modules/renderer/renderer.h"

/**
 * If the current mouse position and the previous position -> reset the previous mouse position
 */
#define MAX_MOUSE_DELTA 10

#define MOUSE_TEST_MOVING_SPEED 5

namespace rpp
{
    f64 InputSystem::m_mouseX = 0.0;
    f64 InputSystem::m_mouseY = 0.0;

    f64 InputSystem::m_previousMouseX = 0.0;
    f64 InputSystem::m_previousMouseY = 0.0;

    void InputSystem::Initialize()
    {
    }

    void InputSystem::Shutdown()
    {
        // Cleanup code for the input system can be added here.
    }

    void InputSystem::Update(f32 delta)
    {
#if defined(RPP_USE_TEST)
        RPP_UNUSED(delta);
#else
        m_previousMouseX = m_mouseX;
        m_previousMouseY = m_mouseY;

        m_mouseX = Renderer::GetWindow()->GetMouseX();
        m_mouseY = Renderer::GetWindow()->GetMouseY();
#endif
    }

    b8 InputSystem::MoveMouseTo(f64 x, f64 y)
    {
#if defined(RPP_USE_TEST)
        if (x == m_mouseX && y == m_mouseY)
        {
            return TRUE;
        }

        m_previousMouseX = m_mouseX;
        m_previousMouseY = m_mouseY;

        f64 xDiff = x - m_mouseX;
        if (abs(xDiff) < MOUSE_TEST_MOVING_SPEED)
        {
            m_mouseX = x;
        }
        else
        {
            m_mouseX += xDiff / abs(xDiff) * MOUSE_TEST_MOVING_SPEED;
        }

        f64 yDiff = y - m_mouseY;
        if (abs(yDiff) < MOUSE_TEST_MOVING_SPEED)
        {
            m_mouseY = y;
        }
        else
        {
            m_mouseY += yDiff / abs(yDiff) * MOUSE_TEST_MOVING_SPEED;
        }

        Renderer::GetWindow()->SetMousePosition(InputSystem::GetMouseX(), InputSystem::GetMouseY());

        return FALSE;
#else
        RPP_UNUSED(x);
        RPP_UNUSED(y);
        return TRUE;
#endif
    }

    void InputSystem::OnMouseMoveCallback(f64 xPos, f64 yPos, void *pUserData)
    {
#if defined(RPP_USE_TEST)
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
