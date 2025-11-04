#include "modules/input/input_internal.h"
#include "modules/renderer/renderer.h"
#include "modules/test_system/test_system.h"

/**
 * If the current mouse position and the previous position -> reset the previous mouse position
 */
#define MAX_MOUSE_DELTA 10

#define MOUSE_TEST_MOVING_SPEED 5

namespace rpp
{
    InputSystem::RendererInputData InputSystem::s_inputSystemsData[MAX_INPUT_SYSTEMS];
    b8 InputSystem::s_startEvent = FALSE;
    HighResTimer InputSystem::s_timer;

    void InputSystem::Initialize()
    {
        memset(s_inputSystemsData, 0, sizeof(RendererInputData) * MAX_INPUT_SYSTEMS);
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
        u32 rendererId = Renderer::GetCurrentRendererId();
        RPP_ASSERT(rendererId < MAX_INPUT_SYSTEMS);
        RendererInputData &data = s_inputSystemsData[rendererId];
        data.m_previousMouseX = data.m_mouseX;
        data.m_previousMouseY = data.m_mouseY;

        data.m_mouseX = Renderer::GetWindow()->GetMouseX();
        data.m_mouseY = Renderer::GetWindow()->GetMouseY();

        memcpy(data.mousePreviousButtonStates, data.mouseButtonStates, sizeof(i32) * MouseButton::MOUSE_BUTTON_COUNT);

        for (i32 i = 0; i < MouseButton::MOUSE_BUTTON_COUNT; i++)
        {
            data.mouseButtonStates[i] = Renderer::GetWindow()->GetMouseButtonState(static_cast<MouseButton>(i));
        }

        memcpy(data.keyboardPreviousButton, data.keyboardButton, sizeof(i32) * KeyboardButton::KEYBOARD_BUTTON_COUNT);

        for (i32 i = 0; i < KeyboardButton::KEYBOARD_BUTTON_COUNT; i++)
        {
            data.keyboardButton[i] = Renderer::GetWindow()->GetKeyboardButtonState(static_cast<KeyboardButton>(i));
        }
#endif
    }

    b8 InputSystem::MoveMouseTo(f64 x, f64 y)
    {
#if defined(RPP_USE_TEST)
        u32 rendererId = Renderer::GetCurrentRendererId();
        RPP_ASSERT(rendererId < MAX_INPUT_SYSTEMS);
        RendererInputData &data = s_inputSystemsData[rendererId];

        if (x == data.m_mouseX && y == data.m_mouseY)
        {
            return TRUE;
        }

        data.m_previousMouseX = data.m_mouseX;
        data.m_previousMouseY = data.m_mouseY;

        f64 xDiff = x - data.m_mouseX;
        f64 yDiff = y - data.m_mouseY;

        if (abs(xDiff) < MOUSE_TEST_MOVING_SPEED)
        {
            data.m_mouseX = x;
        }
        else
        {
            data.m_mouseX += f64(int(xDiff / (abs(xDiff) + abs(yDiff)) * MOUSE_TEST_MOVING_SPEED));
        }

        if (abs(yDiff) < MOUSE_TEST_MOVING_SPEED)
        {
            data.m_mouseY = y;
        }
        else
        {
            data.m_mouseY += f64(int(yDiff / (abs(xDiff) + abs(yDiff)) * MOUSE_TEST_MOVING_SPEED));
        }

        Renderer::GetWindow()->SetMousePosition(InputSystem::GetMouseX(), InputSystem::GetMouseY());

        return FALSE;
#else
        RPP_UNUSED(x);
        RPP_UNUSED(y);
        return TRUE;
#endif
    }

    b8 InputSystem::ClickMouse(i32 button)
    {
#if defined(RPP_USE_TEST)
        u32 rendererId = Renderer::GetCurrentRendererId();
        RPP_ASSERT(rendererId < MAX_INPUT_SYSTEMS);
        RendererInputData &data = s_inputSystemsData[rendererId];

        if (!s_startEvent)
        {
            s_startEvent = TRUE;
            s_timer.Start();
            // Simulate mouse button down
            data.mouseButtonStates[static_cast<i32>(button)] = ButtonState::PRESSED;
            return FALSE;
        }

        while (s_timer.GetElapsedTimeInMilliseconds() < 100)
        {
            return FALSE;
        }

        // Simulate mouse button up
        data.mouseButtonStates[static_cast<i32>(button)] = ButtonState::RELEASED;
        s_startEvent = FALSE;
        return TRUE;
#else
        RPP_UNUSED(button);
        return TRUE;
#endif
    }

    void InputSystem::OnMouseMoveCallback(f64 xPos, f64 yPos, void *pUserData)
    {
#if defined(RPP_USE_TEST)
        Renderer::GetWindow()->SetMousePosition(InputSystem::GetMouseX(), InputSystem::GetMouseY());
#else
        u32 rendererId = Renderer::GetCurrentRendererId();
        RPP_ASSERT(rendererId < MAX_INPUT_SYSTEMS);
        RendererInputData &data = s_inputSystemsData[rendererId];

        if (abs(xPos - data.m_mouseX) > MAX_MOUSE_DELTA || abs(yPos - data.m_mouseY) > MAX_MOUSE_DELTA)
        {
            data.m_previousMouseX = xPos;
            data.m_previousMouseY = yPos;
        }
        else
        {
            data.m_previousMouseX = data.m_mouseX;
            data.m_previousMouseY = data.m_mouseY;
        }

        data.m_mouseX = xPos;
        data.m_mouseY = yPos;

        RPP_LOG_DEBUG("Mouse moved to position ({}, {})", xPos, yPos);

#endif
    }

    f64 InputSystem::GetMouseX()
    {
        u32 rendererId = Renderer::GetCurrentRendererId();
        RPP_ASSERT(rendererId < MAX_INPUT_SYSTEMS);
        RendererInputData &data = s_inputSystemsData[rendererId];
        return data.m_mouseX;
    }

    f64 InputSystem::GetMouseY()
    {
        u32 rendererId = Renderer::GetCurrentRendererId();
        RPP_ASSERT(rendererId < MAX_INPUT_SYSTEMS);
        RendererInputData &data = s_inputSystemsData[rendererId];
        return data.m_mouseY;
    }

    f64 InputSystem::GetDeltaX()
    {
        u32 rendererId = Renderer::GetCurrentRendererId();
        RPP_ASSERT(rendererId < MAX_INPUT_SYSTEMS);
        RendererInputData &data = s_inputSystemsData[rendererId];
        return data.m_mouseX - data.m_previousMouseX;
    }

    f64 InputSystem::GetDeltaY()
    {
        u32 rendererId = Renderer::GetCurrentRendererId();
        RPP_ASSERT(rendererId < MAX_INPUT_SYSTEMS);
        RendererInputData &data = s_inputSystemsData[rendererId];
        return data.m_mouseY - data.m_previousMouseY;
    }

    b8 InputSystem::IsMouseButtonDown(MouseButton button)
    {
        u32 rendererId = Renderer::GetCurrentRendererId();
        RPP_ASSERT(rendererId < MAX_INPUT_SYSTEMS);
        RendererInputData &data = s_inputSystemsData[rendererId];
        return data.mouseButtonStates[static_cast<i32>(button)] == ButtonState::PRESSED;
    }

    b8 InputSystem::IsKeyboardButtonDown(KeyboardButton button)
    {
        u32 rendererId = Renderer::GetCurrentRendererId();
        RPP_ASSERT(rendererId < MAX_INPUT_SYSTEMS);
        RendererInputData &data = s_inputSystemsData[rendererId];
        return data.keyboardButton[static_cast<i32>(button)] == ButtonState::PRESSED;
    }
} // namespace rpp
