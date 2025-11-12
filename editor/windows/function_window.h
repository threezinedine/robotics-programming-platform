#pragma once
#include "applications/applications.h"

namespace rpp
{
    class FunctionWindow : public GraphicSession
    {
    public:
        FunctionWindow(u32 width, u32 height, const String &title);
        ~FunctionWindow();

    protected:
        void InitializeImpl() override;
        void UpdateImpl(f32 deltaTime) override;
        void RenderImpl() override;

    private:
        void ToolbarRender();
    };
} // namespace rpp
