#include "function_window.h"

namespace rpp
{
    FunctionWindow::FunctionWindow(u32 width, u32 height, const String &title, const FunctionDescription &functionDescription)
        : GraphicSession(width, height, title)
    {
        RPP_PROFILE_SCOPE();
        m_pFunctionBlock = RPP_NEW(FunctionBlock, functionDescription);
        m_pFunction = Function::Create(functionDescription);
    }

    FunctionWindow::~FunctionWindow()
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(m_pFunctionBlock != nullptr);
        RPP_DELETE(m_pFunctionBlock);
        m_pFunctionBlock = nullptr;

        RPP_ASSERT(m_pFunction != nullptr); ///< Do not delete the function here, it is managed by the project.
        RPP_DELETE(m_pFunction);
        m_pFunction = nullptr;
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
        Renderer::DrawRectangle({100, 100, 200, 150});

        u32 windowWidth = GetWidth();
        u32 windowHeight = GetHeight();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((f32)windowWidth, (f32)windowHeight));
        ImGui::Begin("Function Window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
        {
            ToolbarRender();

            Renderer::DrawingSceneInImGui();
        }
        ImGui::End();
    }

    void FunctionWindow::ToolbarRender()
    {
        RPP_PROFILE_SCOPE();

        if (ImGui::Button("Test"))
        {
        }

        ImGui::SameLine();

        if (ImGui::Button("Another Button"))
        {
        }
    }
} // namespace rpp
