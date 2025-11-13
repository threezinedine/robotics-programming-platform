#pragma once
#include "applications/applications.h"

namespace rpp
{
    class FunctionWindow : public GraphicSession
    {
    public:
        FunctionWindow(u32 width, u32 height, const String &title, const FunctionDescription &functionDescription);
        ~FunctionWindow();

    protected:
        void InitializeImpl() override;
        void UpdateImpl(f32 deltaTime) override;
        void RenderImpl() override;

    private:
        void ToolbarRender();

    private:
        Function *m_pFunction;           ///< The logic function being represented.
        FunctionBlock *m_pFunctionBlock; ///< This instance manages the function block UI.
    };
} // namespace rpp
