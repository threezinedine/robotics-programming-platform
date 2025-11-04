#include "applications/applications.h"

using namespace rpp;

class EditorWindow : public GraphicSession
{
public:
    EditorWindow(u32 width, u32 height, const String &title);
    ~EditorWindow();

protected:
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

    void RenderEditorMain();

private:
    b8 m_openNewProjectModal = false; ///< Whether to open the New Project modal.

    /**
     * The holded project, if `nullptr` means no project is opened.
     */
    Project *m_pCurrentProject;
};