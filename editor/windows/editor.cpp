#include "editor.h"

#define NEW_PROJECT_MODAL_ID "New Project Modal"
#define EDITOR_DATA_FILE "editor.json"

EditorWindow::EditorWindow(u32 width, u32 height, const String &title)
    : GraphicSession(width, height, title),
      m_pCurrentProject(nullptr),
      m_pEditorData(nullptr),
      m_openProjectFile("")
{
    RPP_PROFILE_SCOPE();
    if (FileSystem::PathExists(EDITOR_DATA_FILE))
    {
        m_pEditorData = EditorData::Create(EDITOR_DATA_FILE);
        RPP_LOG_INFO("Loaded editor data from editor.json.");
    }
    else
    {
        m_pEditorData = EditorData::Create();
        m_pEditorData->Save(EDITOR_DATA_FILE);
    }

    memset(m_editedFunctionName, 0, sizeof(m_editedFunctionName));
}

EditorWindow::~EditorWindow()
{
    RPP_PROFILE_SCOPE();
    RPP_DELETE(m_pEditorData);
}

void EditorWindow::InitializeImpl()
{
    RPP_PROFILE_SCOPE();
    RPP_ASSERT(m_pEditorData != nullptr);

    if (m_pEditorData->GetRecentProjects().Size() > 0)
    {
        OpenProject(m_pEditorData->GetRecentProjects()[0]);
    }

    Renderer::GetWindow()->SetOnCloseCallback(
        [&](void *pUserData)
        {
            RPP_PROFILE_SCOPE();
            EditorWindow *pEditor = static_cast<EditorWindow *>(pUserData);
            RPP_ASSERT(pEditor != nullptr);

            if (m_pCurrentProject == nullptr)
            {
                return;
            }

            if (HistoryManager::GetInstance()->Empty())
            {
                return;
            }

            m_openUnsavedChangesModal = TRUE;
            GraphicsCommandData preventCloseCommandData = {};
            preventCloseCommandData.type = GraphicsCommandType::PREVENT_CLOSE;
            Renderer::GetWindow()->ExecuteCommand(preventCloseCommandData);
        });

    HistoryManager::GetInstance()->SetOnCommandExecuteCallback(
        [this](Command *pCommand)
        {
            RPP_PROFILE_SCOPE();
            Renderer::SetWindowTitle(Format("Editor - {}*", m_pCurrentProject->GetName()));
        });

    HistoryManager::GetInstance()->SetOnHistoryEmptyCallback(
        [this]()
        {
            RPP_PROFILE_SCOPE();
            Renderer::SetWindowTitle(Format("Editor - {}", m_pCurrentProject->GetName()));
        });
}

void EditorWindow::RenderImpl()
{
    RPP_PROFILE_SCOPE();
    ImGuiIO &io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));

    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
    {
        MenuRender();

        EditorMainRender();

        NewProjectModalRender();
    }
    ImGui::End();
}

void EditorWindow::MenuRender()
{
    RPP_PROFILE_SCOPE();
    ImGui::BeginMenuBar();
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::BeginMenu("New"))
            {
                if (ImGui::MenuItem("Project##New"))
                {
                    m_openNewProjectModal = true;
                }
                RPP_MARK_ITEM("Editor/MenuBar/File/New/Project");

                if (ImGui::MenuItem("File##New"))
                {
                }
                RPP_MARK_ITEM("Editor/MenuBar/File/New/File");

                ImGui::EndMenu();
            }
            RPP_MARK_ITEM("Editor/MenuBar/File/New");

            if (ImGui::BeginMenu("Open"))
            {
                if (ImGui::MenuItem("Project##Open"))
                {
#if defined(RPP_USE_TEST)
                    m_openOpenProjectTestModal = true;
#else
                    IGFD::FileDialogConfig config;
                    config.path = ".";
                    ImGuiFileDialog::Instance()->OpenDialog("OpenProjectDlgKey", "Open Project", ".rppproj", config);
#endif
                }
                RPP_MARK_ITEM("Editor/MenuBar/File/Open/Project");

                if (ImGui::MenuItem("File##Open"))
                {
                }
                RPP_MARK_ITEM("Editor/MenuBar/File/Open/File");

                ImGui::EndMenu();
            }
            RPP_MARK_ITEM("Editor/MenuBar/File/Open");

            ImGui::Separator();

            u32 recentProjectsCount = m_pEditorData->GetRecentProjects().Size();

            ImGui::BeginDisabled(recentProjectsCount == 0);
            if (ImGui::BeginMenu("Recents"))
            {
                for (u32 recentIndex = 0u; recentIndex < recentProjectsCount; recentIndex++)
                {
                    String recentProjectPath = m_pEditorData->GetRecentProjects()[recentIndex];
                    if (ImGui::MenuItem(recentProjectPath.CStr()))
                    {
                        OpenProject(recentProjectPath);
                    }
                    RPP_MARK_ITEM(Format("Editor/MenuBar/File/Recents/RecentProject{}", recentIndex));
                }

                ImGui::EndMenu();
            }
            RPP_MARK_ITEM("Editor/MenuBar/File/Recents");
            ImGui::EndDisabled();

            ImGui::Separator();

            if (ImGui::MenuItem("Save"))
            {
                SaveProject();
                HistoryManager::GetInstance()->Reset();
            }
            RPP_MARK_ITEM("Editor/MenuBar/File/Save");

            ImGui::EndMenu();
        }
        RPP_MARK_ITEM("Editor/MenuBar/File");
    }
    ImGui::EndMenuBar();

#if defined(RPP_USE_TEST)
    if (m_openOpenProjectTestModal)
    {
        ImGui::OpenPopup("OpenProjectTestModal");
        RPP_LOG_DEBUG("Opened OpenProjectTestModal popup.");
        m_openOpenProjectTestModal = false;
    }

    if (ImGui::BeginPopupModal("OpenProjectTestModal", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        static char projectPath[512] = "";
        ImGui::InputText("Path##OpenProjectTestModal", projectPath, sizeof(projectPath));
        RPP_MARK_ITEM("Editor/OpenProjectTestModal/PathInput", projectPath);

        if (ImGui::Button("Ok"))
        {
            OpenProject(String(projectPath));
            ImGui::CloseCurrentPopup();
        }
        RPP_MARK_ITEM("Editor/OpenProjectTestModal/OK");

        ImGui::EndPopup();
        RPP_MARK_ITEM("Editor/OpenProjectTestModal");
    }
#else
    if (ImGuiFileDialog::Instance()->Display("OpenProjectDlgKey"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            OpenProject(String(filePathName.c_str()));
        }

        ImGuiFileDialog::Instance()->Close();
    }
#endif
}

void EditorWindow::NewProjectModalRender()
{
    RPP_PROFILE_SCOPE();
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
    RPP_PROFILE_SCOPE();
    m_pCurrentProject = Project::Create(desc);

    String finalProjectPath = Format("{}/{}", projectFolder, desc.name);
    String projectFilePath = Format("{}/project.rppproj", finalProjectPath);

    FileSystem::CreateDirectory(finalProjectPath);
    m_pCurrentProject->Save(projectFilePath);
    m_pEditorData->AddRecentProject(projectFilePath);
    m_pEditorData->Save(EDITOR_DATA_FILE);

    RPP_ASSERT(FileSystem::PathExists(projectFilePath));
    Renderer::SetWindowTitle(Format("Editor - {}", desc.name));
}

void EditorWindow::OpenProject(const String &projectFilePath)
{
    RPP_PROFILE_SCOPE();
    RPP_ASSERT(FileSystem::PathExists(projectFilePath));

    FileHandle file = FileSystem::OpenFile(projectFilePath, FILE_MODE_READ);
    String fileContent = FileSystem::Read(file);
    FileSystem::CloseFile(file);

    ProjectDescription desc = FromString<ProjectDescription>(fileContent);

    if (m_pCurrentProject != nullptr)
    {
        RPP_DELETE(m_pCurrentProject);
        m_pCurrentProject = nullptr;
    }

    m_pCurrentProject = RPP_NEW(Project, desc);
    RPP_LOG_DEBUG("Opened project: {}", projectFilePath);
    m_pEditorData->AddRecentProject(projectFilePath);
    m_pEditorData->Save(EDITOR_DATA_FILE);
    m_openProjectFile = projectFilePath;

    Renderer::SetWindowTitle(Format("Editor - {}", desc.name));
}

void EditorWindow::SaveProject()
{
    RPP_PROFILE_SCOPE();
    RPP_ASSERT(m_pCurrentProject != nullptr);

    m_pCurrentProject->Save(m_openProjectFile);
}

void EditorWindow::EditorMainRender()
{
    RPP_PROFILE_SCOPE();
    if (m_pCurrentProject == nullptr)
    {
        ImGui::Text("No project is opened.");
        RPP_MARK_ITEM("Editor/NoProjectOpen");
        return;
    }

    EditorMainToolbarRender();

    if (m_focusFunctionNameInput)
    {
        ImGui::SetNextItemOpen(TRUE);
    }

    if (ImGui::CollapsingHeader("Files"))
    {
        u32 functionCount = m_pCurrentProject->GetFunctionNames().Size();

        bool isFunctionAdded = FALSE;

        for (u32 functionIndex = 0; functionIndex < functionCount; functionIndex++)
        {
            String &functionName = m_pCurrentProject->GetFunctionNames()[functionIndex];

            if (m_currentEditingFunctionIndex == functionIndex)
            {
                if (m_focusFunctionNameInput)
                {
                    ImGui::SetKeyboardFocusHere();
                    m_focusFunctionNameInput = FALSE;
                }

                if (ImGui::InputText("###EditingFunctionName", m_editedFunctionName, sizeof(m_editedFunctionName), ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    m_currentEditingFunctionIndex = INVALID_ID;
                    if (String(m_editedFunctionName) != functionName)
                    {
                        isFunctionAdded = TRUE;
                    }
                }
                RPP_MARK_ITEM(Format("Editor/Files/Function/Rename/{}", functionName));
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                ImGui::Selectable(functionName.CStr());
                RPP_MARK_ITEM(Format("Editor/Files/Function/{}", functionName));
                ImGui::PopStyleColor();
            }
        }

        if (isFunctionAdded)
        {
            m_pCurrentProject->GetFunctionNames()[m_currentEditingFunctionIndex] = m_editedFunctionName;
            memset(m_editedFunctionName, 0, sizeof(m_editedFunctionName));
        }
    }
    RPP_MARK_ITEM("Editor/Files");

    UnsavedChangesModalRender();
}

void EditorWindow::UnsavedChangesModalRender()
{
    if (m_openUnsavedChangesModal)
    {
        RPP_LOG_DEBUG("Opening UnsavedChangesModal popup.");
        ImGui::OpenPopup("UnsavedChangesModal");
        m_openUnsavedChangesModal = FALSE;
    }

    ImGuiIO &io = ImGui::GetIO();
    ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("UnsavedChangesModal", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        ImGui::Text("You have unsaved changes. Do you want to save before exiting?");
        ImGui::Separator();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }
        RPP_MARK_ITEM("Editor/UnsavedChangesModal/Cancel");

        ImGui::SameLine();

        if (ImGui::Button("Save"))
        {
            RPP_ASSERT(m_pCurrentProject != nullptr);
            m_pCurrentProject->Save(m_openProjectFile);
#if !defined(RPP_USE_TEST)
            GraphicsCommandData cmdData;
            cmdData.type = GraphicsCommandType::CLOSE_WINDOW;
            Renderer::GetWindow()->ExecuteCommand(cmdData);
#endif
        }
        RPP_MARK_ITEM("Editor/UnsavedChangesModal/Save");
        ImGui::EndPopup();
        RPP_MARK_ITEM("Editor/UnsavedChangesModal");
    }
}

void EditorWindow::EditorMainToolbarRender()
{
    RPP_PROFILE_SCOPE();
    if (ImGui::Button("New"))
    {
        ImGui::OpenPopup("Editor/Main/Toolbar/New/Popup");
    }
    RPP_MARK_ITEM("Editor/Main/Toolbar/New");

    ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
    if (ImGui::BeginPopup("Editor/Main/Toolbar/New/Popup"))
    {
        if (ImGui::MenuItem("Function"))
        {
            RPP_ASSERT(m_pCurrentProject != nullptr);
#if 0
            m_pCurrentProject->AddNewFunction();
#else
            HistoryManager::GetInstance()->ExecuteCommand(RPP_NEW(AddFunctionCommand, m_pCurrentProject));
#endif
            m_currentEditingFunctionIndex = m_pCurrentProject->GetFunctionNames().Size() - 1;
            const char *newFunctionName = m_pCurrentProject->GetFunctionNames()[m_currentEditingFunctionIndex].CStr();
            memcpy(m_editedFunctionName, newFunctionName, strlen(newFunctionName) + 1);
            m_focusFunctionNameInput = TRUE;
        }
        RPP_MARK_ITEM("Editor/Main/Toolbar/New/Popup/Function");

        ImGui::EndPopup();
    }
    RPP_MARK_ITEM("Editor/Main/Toolbar/New/Popup");

    ImGui::SameLine();

    if (ImGui::Button("Reload"))
    {
    }
    RPP_MARK_ITEM("Editor/Main/Toolbar/Reload");

    ImGui::Separator();
}

void EditorWindow::ShutdownImpl()
{
    RPP_PROFILE_SCOPE();
    if (m_pCurrentProject != nullptr)
    {
        RPP_DELETE(m_pCurrentProject);
        m_pCurrentProject = nullptr;
    }
}