#include "applications/applications.h"

using namespace rpp;

class EditorWindow : public GraphicSession
{
public:
    EditorWindow(u32 width, u32 height, const String &title);
    ~EditorWindow();

protected:
    void InitializeImpl() override;

    void RenderImpl() override;

    void ShutdownImpl() override;

private:
    /**
     * Drawing the tools bar.
     */
    void MenuRender();

    /**
     * Drawing the New Project modal.
     */
    void NewProjectModalRender();

    void EditorMainRender();

    void EditorMainToolbarRender();

private:
    void CreateProject(const String &projectFolder, const ProjectDescription &desc);

    void OpenProject(const String &projectFilePath);

    void SaveProject();

private:
    b8 m_openNewProjectModal = FALSE; ///< Whether to open the New Project modal.

#if defined(RPP_USE_TEST)
    b8 m_openOpenProjectTestModal = FALSE; ///< Whether to open the Open Project test modal. Only used in test builds.
#endif

    /**
     * The holded project, if `nullptr` means no project is opened.
     */
    Project *m_pCurrentProject;

    /**
     * The global data for the editor application, be loadded each time the editor is started.
     */
    EditorData *m_pEditorData;

    char m_editedFunctionName[256] = "";            ///< The name of the function currently being edited.
    u32 m_currentEditingFunctionIndex = INVALID_ID; ///< The index of the currently editing function.
    b8 m_focusFunctionNameInput = FALSE;            ///< Whether to focus the function name input box.

    String m_openProjectFile; ///< The folder path of the currently opened project.
};