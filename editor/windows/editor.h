#include "applications/applications.h"

using namespace rpp;

class EditorWindow : public GraphicSession
{
public:
    EditorWindow(u32 width, u32 height, const String &title);
    ~EditorWindow();

protected:
    void RenderImpl() override;

private:
    /**
     * Drawing the tools bar.
     */
    void MenuRender();

    /**
     * Drawing the New Project modal.
     */
    void NewProjectModalRender();

private:
    b8 m_openNewProjectModal = false;
};