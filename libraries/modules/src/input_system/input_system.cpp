#include "modules/input_system/input_system.h"

namespace rpp
{
    // Static member definitions
    InputSystem::KeyState InputSystem::s_keyStatus[RPP_MAX_KEYS];
    InputSystem::MouseButtonState InputSystem::s_mouseButtonStatus[RPP_MAX_MOUSE_BUTTONS];
    Scope<Array<KeyboardEventCallback>> InputSystem::s_keyboardEventCallbacks = nullptr;
    Scope<Array<MouseEventCallback>> InputSystem::s_mouseEventCallbacks = nullptr;
    i32 InputSystem::s_mouseX = 0;
    i32 InputSystem::s_mouseY = 0;

    void InputSystem::Initialize()
    {
        /// System checking
        RPP_ASSERT_MSG(static_cast<u32>(Key::COUNT) <= RPP_MAX_KEYS, "RPP_MAX_KEYS is too small to hold all key codes");
        RPP_ASSERT_MSG(static_cast<u32>(MouseButton::COUNT) <= RPP_MAX_MOUSE_BUTTONS, "RPP_MAX_MOUSE_BUTTONS is too small to hold all mouse button codes");

        RPP_ASSERT(s_keyboardEventCallbacks == nullptr);
        s_keyboardEventCallbacks = CreateScope<Array<KeyboardEventCallback>>();

        RPP_ASSERT(s_mouseEventCallbacks == nullptr);
        s_mouseEventCallbacks = CreateScope<Array<MouseEventCallback>>();

        for (u32 i = 0; i < RPP_MAX_KEYS; ++i)
        {
            s_keyStatus[i].key = static_cast<Key>(i);
            s_keyStatus[i].state = FALSE;
            s_keyStatus[i].previousState = FALSE;
        }

        for (u32 i = 0; i < RPP_MAX_MOUSE_BUTTONS; ++i)
        {
            s_mouseButtonStatus[i].button = static_cast<MouseButton>(i);
            s_mouseButtonStatus[i].state = FALSE;
            s_mouseButtonStatus[i].previousState = FALSE;
        }
    }

    void InputSystem::Shutdown()
    {
        RPP_ASSERT(s_keyboardEventCallbacks != nullptr);
        s_keyboardEventCallbacks = nullptr;

        RPP_ASSERT(s_mouseEventCallbacks != nullptr);
        s_mouseEventCallbacks = nullptr;
    }

    b8 InputSystem::GetKeyState(Key key)
    {
        RPP_ASSERT(key < Key::COUNT);
        return s_keyStatus[static_cast<u32>(key)].state;
    }

    b8 InputSystem::GetKeyPreviousState(Key key)
    {
        RPP_ASSERT(key < Key::COUNT);
        return s_keyStatus[static_cast<u32>(key)].previousState;
    }

    b8 InputSystem::GetMouseState(MouseButton button)
    {
        RPP_ASSERT(button < MouseButton::COUNT);
        return s_mouseButtonStatus[static_cast<u32>(button)].state;
    }

    b8 InputSystem::GetMousePreviousState(MouseButton button)
    {
        RPP_ASSERT(button < MouseButton::COUNT);
        return s_mouseButtonStatus[static_cast<u32>(button)].previousState;
    }

    i32 InputSystem::GetMouseX()
    {
        return s_mouseX;
    }

    i32 InputSystem::GetMouseY()
    {
        return s_mouseY;
    }

    void InputSystem::RegisterKeyboardEventCallback(KeyboardEventCallback callback)
    {
        RPP_ASSERT(s_keyboardEventCallbacks != nullptr);
        s_keyboardEventCallbacks->Push(callback);
    }

    void InputSystem::RegisterMouseEventCallback(MouseEventCallback callback)
    {
        RPP_ASSERT(s_mouseEventCallbacks != nullptr);
        s_mouseEventCallbacks->Push(callback);
    }

    void InputSystem::Update()
    {
    }

    void InputSystem::SetKeyState(Key key, b8 pressed)
    {
        RPP_ASSERT(key < Key::COUNT);
        u32 index = static_cast<u32>(key);
        s_keyStatus[index].previousState = s_keyStatus[index].state;
        s_keyStatus[index].state = pressed;
    }

    void InputSystem::SetMouseState(MouseButton button, b8 pressed, i32 x, i32 y)
    {
        RPP_ASSERT(button < MouseButton::COUNT);
        u32 index = static_cast<u32>(button);
        s_mouseButtonStatus[index].previousState = s_mouseButtonStatus[index].state;
        s_mouseButtonStatus[index].state = pressed;
    }
} // namespace rpp
