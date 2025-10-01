#include "applications/applications.h"
#include "glm/gtc/matrix_transform.hpp"

RPP_ENABLE_MEMORY_TRACKING;

using namespace rpp;

class FirstWindow : public GraphicSession
{
public:
    FirstWindow(u32 width, u32 height, const String &title, b8 enableImGui = FALSE)
        : GraphicSession(width, height, title, enableImGui)
    {
    }

    ~FirstWindow()
    {
    }

protected:
    void RenderImpl() override
    {
        ImGui::Begin("First Window");
        ImGui::Text("This is the first window.");
        ImGui::End();
    }
};

class TestSession : public GraphicSession
{
public:
    TestSession(u32 width, u32 height, const String &title, b8 enableImGui = FALSE)
        : GraphicSession(width, height, title, enableImGui)
    {
    }

    ~TestSession()
    {
    }

protected:
    void RenderImpl() override
    {
        Renderer::DrawRectangle({0, 0, 100, 100});
    }
};

int main(void)
{
    SingletonManager::Initialize();
    Logging::GetInstance()->Setup(u8(HandlerType::CONSOLE), LogLevel::DEBUG);
    Renderer::Initialize();

    {
        CREATE_SESSION(FirstWindow, 800, 600, "Test2", TRUE);
        CREATE_SESSION(TestSession, 1000, 1000, "Test");

        while (TRUE)
        {
            if (GraphicSessionManager::GetInstance()->Update(0.0f))
            {
                break;
            }
        }
    }

    GraphicSessionManager::GetInstance()->ClearSessions();
    Renderer::Shutdown();
    SingletonManager::Shutdown();
    return 0;
}
