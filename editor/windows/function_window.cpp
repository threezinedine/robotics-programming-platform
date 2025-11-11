#include "function_window.h"

namespace rpp
{
    FunctionWindow::FunctionWindow(u32 width, u32 height, const String &title)
        : GraphicSession(width, height, title)
    {
        RPP_PROFILE_SCOPE();
    }

    FunctionWindow::~FunctionWindow()
    {
        RPP_PROFILE_SCOPE();
    }

    void FunctionWindow::InitializeImpl()
    {
        RPP_PROFILE_SCOPE();
    }

    void FunctionWindow::UpdateImpl(f32 deltaTime)
    {
        RPP_PROFILE_SCOPE();
        RPP_UNUSED(deltaTime);
    }

    void FunctionWindow::RenderImpl()
    {
        RPP_PROFILE_SCOPE();
    }
} // namespace rpp
