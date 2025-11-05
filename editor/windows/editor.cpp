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
    ImGuiIO &io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));

    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
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
        static char errorMessage[512] = "";

        ImGui::InputText("Project Name"
                         "##NewProjectModal",
                         projectName, sizeof(projectName));
        RPP_MARK_ITEM("Editor/NewProjectModal/ProjectNameInput", projectName);

#if defined(RPP_USE_TEST)
        ImGui::InputText("Folder"
                         "##NewProjectModal",
                         projectFolder, sizeof(projectFolder));
        RPP_MARK_ITEM("Editor/NewProjectModal/ProjectFolderInput");
#else
        ImGui::Text(projectFolder == "" ? "Folder: " : "Folder: %s", projectFolder);
        ImGui::SameLine();
        if (ImGui::Button("Browse"))
        {
            IGFD::FileDialogConfig config;
            config.path = ".";
            ImGuiFileDialog::Instance()->OpenDialog("ChooseProjectFolderDlgKey", "Choose Project Folder", nullptr, config);
        }

        if (ImGuiFileDialog::Instance()->Display("ChooseProjectFolderDlgKey"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string filePathName = ImGuiFileDialog::Instance()->GetCurrentPath();
                snprintf(projectFolder, sizeof(projectFolder), "%s", filePathName.c_str());
            }

            ImGuiFileDialog::Instance()->Close();
        }
#endif

        if (strcmp(errorMessage, "") != 0)
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", errorMessage);
            RPP_MARK_ITEM("Editor/NewProjectModal/ErrorMessage");
        }

        ImGui::Separator();

        if (ImGui::Button("Cancel"))
        {
            memset(projectName, 0, sizeof(projectName));
            memset(projectFolder, 0, sizeof(projectFolder));
            memset(errorMessage, 0, sizeof(errorMessage));

            ImGui::CloseCurrentPopup();
        }
        RPP_MARK_ITEM("Editor/NewProjectModal/Cancel");

        ImGui::SameLine();

        if (ImGui::Button("Create"))
        {
            String finalProjectPath = Format("{}/{}", String(projectFolder), String(projectName));

            b8 isProjectNameValid = strcmp(projectName, "") != 0 && !FileSystem::PathExists(finalProjectPath);
            b8 isProjectFolderValid = strcmp(projectFolder, "") != 0 && FileSystem::PathExists(String(projectFolder));

            if (isProjectNameValid && isProjectFolderValid)
            {
                ProjectDescription desc;
                desc.name = String(projectName);
                CreateProject(projectFolder, desc);

                ImGui::CloseCurrentPopup();
            }
            else
            {
                // ImGui::InsertNotification({ImGuiToastType_Error, 3000, "Please fill in all fields before creating a new project."});
                if (!isProjectNameValid)
                {
                    snprintf(errorMessage, sizeof(errorMessage), "Invalid project name or project already exists.");
                }
                else if (!isProjectFolderValid)
                {
                    snprintf(errorMessage, sizeof(errorMessage), "Invalid project folder or folder does not exist.");
                }
            }
        }
        RPP_MARK_ITEM("Editor/NewProjectModal/Create");

        ImGui::EndPopup();
        RPP_MARK_ITEM("Editor/NewProjectModal");
    }
}

void EditorWindow::CreateProject(const String &projectFolder, const ProjectDescription &desc)
{
    m_pCurrentProject = Project::CreateProject(desc);

    String finalProjectPath = Format("{}/{}", projectFolder, desc.name);
    String projectFilePath = Format("{}/project.rppproj", finalProjectPath);

    FileSystem::CreateDirectory(finalProjectPath);
    RPP_LOG_DEBUG("Project Path: {} - Project file: {}", finalProjectPath, projectFilePath);
    FileHandle file = FileSystem::OpenFile(projectFilePath, FILE_MODE_WRITE);
    FileSystem::Write(file, ToString(desc));
    FileSystem::CloseFile(file);

    RPP_ASSERT(FileSystem::PathExists(projectFilePath));
    Renderer::SetWindowTitle(Format("Editor - {}", desc.name));
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