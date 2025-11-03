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
};