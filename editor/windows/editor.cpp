#include "editor.h"

EditorWindow::EditorWindow(u32 width, u32 height, const String &title)
    : GraphicSession(width, height, title)
{
}

EditorWindow::~EditorWindow()
{
}

void EditorWindow::RenderImpl()
{
    ImGui::Begin("Test Window");

    if (ImGui::Button("Click Me"))
    {
        RPP_LOG_INFO("Click Me button pressed!");
    }

    if (ImGui::Button("Test Button"))
    {
        RPP_LOG_INFO("Test Button pressed!");
    }

    ImGui::End();
}