#include "editor.h"

#define NEW_PROJECT_MODAL_ID "New Project Modal"

EditorWindow::EditorWindow(u32 width, u32 height, const String &title)
    : GraphicSession(width, height, title),
      m_pCurrentProject(nullptr)
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

    // center the modal
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal(NEW_PROJECT_MODAL_ID, NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        static char projectName[256] = "";
        static char projectFolder[512] = "";
        ImGui::InputText("Project Name"
                         "##NewProjectModal",
                         projectName, sizeof(projectName));
        RPP_MARK_ITEM("Editor/NewProjectModal/ProjectNameInput", projectName);

#if !defined(RPP_USE_TEST)
        ImGui::BeginDisabled();
#endif
        ImGui::InputText("Folder"
                         "##NewProjectModal",
                         projectFolder, sizeof(projectFolder));
        RPP_MARK_ITEM("Editor/NewProjectModal/ProjectFolderInput");
#if !defined(RPP_USE_TEST)
        ImGui::EndDisabled();
#endif
        ImGui::SameLine();
        if (ImGui::Button("Browse"))
        {
        }

        if (ImGui::Button("Cancel"))
        {
            memset(projectName, 0, sizeof(projectName));
            memset(projectFolder, 0, sizeof(projectFolder));

            ImGui::CloseCurrentPopup();
        }
        RPP_MARK_ITEM("Editor/NewProjectModal/Cancel");

        ImGui::SameLine();

        if (ImGui::Button("Create"))
        {
            b8 isProjectNameValid = strcmp(projectName, "") != 0;
            b8 isProjectFolderValid = strcmp(projectFolder, "") != 0;

            if (isProjectFolderValid && isProjectFolderValid)
            {
                ProjectDescription desc;
                desc.name = String(projectName);
                m_pCurrentProject = Project::CreateProject(desc);
            }
        }
        RPP_MARK_ITEM("Editor/NewProjectModal/Create");

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

void EditorWindow::ShutdownImpl()
{
    if (m_pCurrentProject != nullptr)
    {
        RPP_DELETE(m_pCurrentProject);
        m_pCurrentProject = nullptr;
    }
}