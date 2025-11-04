#include "editor.h"

#define NEW_PROJECT_MODAL_ID "New Project Modal"

EditorWindow::EditorWindow(u32 width, u32 height, const String &title)
    : GraphicSession(width, height, title)
{
}

EditorWindow::~EditorWindow()
{
}

void EditorWindow::RenderImpl()
{
    ;
    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar);
    {
        MenuRender();

        RenderEditorMain();

        NewProjectModalRender();
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
                    m_openNewProjectModal = true;
                }
                RPP_MARK_ITEM("Editor/MenuBar/File/New/Project");

                if (ImGui::MenuItem("File"))
                {
                }
                RPP_MARK_ITEM("Editor/MenuBar/File/New/File");

                ImGui::EndMenu();
            }
            RPP_MARK_ITEM("Editor/MenuBar/File/New");

            ImGui::EndMenu();
        }
        RPP_MARK_ITEM("Editor/MenuBar/File");
    }
    ImGui::EndMenuBar();
}

void EditorWindow::NewProjectModalRender()
{
    if (m_openNewProjectModal)
    {
        ImGui::OpenPopup(NEW_PROJECT_MODAL_ID);
        m_openNewProjectModal = false;
    }

    if (ImGui::BeginPopupModal(NEW_PROJECT_MODAL_ID, NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }
        RPP_MARK_ITEM("Editor/NewProjectModal/Cancel");

        ImGui::EndPopup();
        RPP_MARK_ITEM("Editor/NewProjectModal");
    }
}

void EditorWindow::RenderEditorMain()
{
    static char projectName[256] = "";
    ImGui::InputText("Project Name", projectName, sizeof(projectName));
    RPP_MARK_ITEM("Editor/TestInput");

    ImGui::Text(Format("A state: {}", InputSystem::IsKeyboardButtonDown(KeyboardButton::KEY_A)).CStr());
}