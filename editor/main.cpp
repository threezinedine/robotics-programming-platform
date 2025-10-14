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
#if defined(RPP_USE_TEST)
    FileSystem::Initialize("temp");
#else
    FileSystem::Initialize();
#endif
    Logging::GetInstance()->Setup(u8(HandlerType::CONSOLE), LogLevel::DEBUG);
    Renderer::Initialize();
    Thread::Initialize();
    Signal::Initialize();

#if defined(RPP_USE_TEST)
    TestSystem::GetInstance()->Initialize(
        String(STRINGIFY(RPP_PROJECT_DIR) "/e2e-gruntime/results.json"),
        String(""),
        String(STRINGIFY(RPP_PROJECT_DIR) "/e2e-gruntime/empty_scenario.py"));
#endif

    {
        CREATE_SESSION(FirstWindow, 800, 600, "Test2", TRUE);
        CREATE_SESSION(TestSession, 1000, 1000, "Test");

        while (TRUE)
        {
#if defined(RPP_USE_TEST)
            if (!TestSystem::GetInstance()->ShouldApplicationClose())
            {
                TestSystem::GetInstance()->Update(0.0f);
            }
            else
            {
                // If the test system indicates to close, break the loop to exit the application.
                break;
            }
#endif

            if (GraphicSessionManager::GetInstance()->Update(0.0f))
            {
                break;
            }

#if defined(RPP_USE_TEST)
            if (TestSystem::GetInstance()->ShouldApplicationClose())
            {
                // Close all the sessions when the test thread ends
                u32 sessionCount = GraphicSessionManager::GetInstance()->GetSessionCount();
                for (u32 i = 0; i < sessionCount; i++)
                {
                    u32 rendererId = GraphicSessionManager::GetInstance()->GetSession(i)->GetRendererId();
                    Renderer::Activate(rendererId);
                    Renderer::CloseWindow();
                }
            }
#endif
        }
    }

#if defined(RPP_USE_TEST)
    TestSystem::GetInstance()->Shutdown();
#endif

    Signal::Shutdown();
    Thread::Shutdown();
    GraphicSessionManager::GetInstance()->ClearSessions();
    Renderer::Shutdown();
    FileSystem::Shutdown();
    SingletonManager::Shutdown();
    return 0;
}
