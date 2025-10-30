#include "modules/input/input_internal.h"
#include "modules/renderer/renderer.h"

namespace rpp
{
    f64 InputSystem::m_mouseX = 0.0;
    f64 InputSystem::m_mouseY = 0.0;

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
    }

    void InputSystem::OnMouseMoveCallback(f64 xPos, f64 yPos, void *pUserData)
    {
#if defined(RPP_USE_TEST)
        m_mouseX = 100;
        m_mouseY = 200;

        Renderer::GetWindow()->SetMousePosition(InputSystem::GetMouseX(), InputSystem::GetMouseY());
#else
        m_mouseX = xPos;
        m_mouseY = yPos;
#endif
    }
} // namespace rpp
