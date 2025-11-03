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
    ImGui::ShowDemoWindow();

    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar);
    {
        MenuRender();

        ImGui::Text("Hello, Editor!");
    }
    ImGui::End();
}

void EditorWindow::MenuRender()
{
    ImGui::BeginMenuBar();
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::BeginMenu("New"))
            {
                if (ImGui::MenuItem("Project"))
                {
                    RPP_LOG_INFO("New Project created!");
                }
                RPP_MARK_ITEM("Editor/MenuBar/File/New/Project");

                if (ImGui::MenuItem("File"))
                {
                    RPP_LOG_INFO("New File created!");
                }

                ImGui::EndMenu();
            }
            RPP_MARK_ITEM("Editor/MenuBar/File/New");

            ImGui::EndMenu();
        }
        RPP_MARK_ITEM("Editor/MenuBar/File");
    }
    ImGui::EndMenuBar();
}