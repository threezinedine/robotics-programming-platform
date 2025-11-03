#include "applications/applications.h"
#include "windows/editor.h"

RPP_ENABLE_MEMORY_TRACKING;

using namespace rpp;

int main(int argc, char **argv)
{
    ArgParser parser;

#if defined(RPP_USE_TEST)
    parser.AddArgument({"runtime"});
#endif

    Json args = parser.Parse(argc, argv);

    SingletonManager::Initialize();
#if defined(RPP_USE_TEST)
    FileSystem::Initialize("temp");
#else
    FileSystem::Initialize();
#endif
    Logging::GetInstance()->Setup(u8(HandlerType::CONSOLE), LogLevel::DEBUG);

    InputSystem::Initialize();
    Renderer::Initialize();
    Thread::Initialize();
    Signal::Initialize();

#if defined(RPP_USE_TEST)
    String runtimeFilePath = Format("{}/e2e/{}.py", String(STRINGIFY(RPP_PROJECT_DIR)), args.Get<String>("runtime", "basic"));

    TestSystem::GetInstance()->Initialize(
        String(STRINGIFY(RPP_PROJECT_DIR) "/e2e/results.json"),
        String(""),
        runtimeFilePath);
#endif

    {
        CREATE_SESSION(EditorWindow, 400, 600, "Editor");

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
    InputSystem::Shutdown();
    FileSystem::Shutdown();
    SingletonManager::Shutdown();
    return 0;
}
