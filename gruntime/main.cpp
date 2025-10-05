#include "platforms/memory.h"
RPP_ENABLE_MEMORY_TRACKING;

#include "applications/applications.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace rpp;

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
    void InitializeImpl() override
    {
    }

    void RenderImpl() override
    {
        Renderer::DrawRectangle({0, 0, 100, 100});
        Renderer::DrawCircle({50, 50}, 50);

        ImGui::ShowDemoWindow();

        ImGui::Begin("My Captured Scene Window");
        Renderer::DrawingSceneInImGui();
        ImGui::End();
    }

    void ShutdownImpl() override
    {
        m_texture = INVALID_ID;
    }

private:
    u32 m_texture = INVALID_ID;
};

int main(void)
{
    SingletonManager::Initialize();
    FileSystem::Initialize();
    Logging::GetInstance()->Setup(u8(HandlerType::CONSOLE), LogLevel::DEBUG);
    Renderer::Initialize();
    Thread::Initialize();
    Signal::Initialize();

<<<<<<< HEAD
    SignalTestParam testParam = {};
    testParam.mainSignal = Signal::Create();
    testParam.threadSignal = Signal::Create();

    ThreadId signalThreadId = Thread::Create(SignalTestThread, &testParam, sizeof(testParam));

    Thread::Start(signalThreadId); // Start the thread

    Signal::Wait(testParam.mainSignal); // Wait for the thread to notify
    print("Push 4\n", ConsoleColor::YELLOW);
    Signal::Active(testParam.threadSignal); // Activate the signal to let the thread proceed

    Signal::Wait(testParam.mainSignal); // Wait for the thread to notify
    print("Push 6\n", ConsoleColor::YELLOW);
    Signal::Active(testParam.threadSignal); // Activate the signal to let the thread proceed

    Signal::Wait(testParam.mainSignal); // Wait for the thread to notify

    Thread::Destroy(signalThreadId);

    print("\n");

    // FileHandle file = FileSystem::OpenFile("");
    // if (FileSystem::IsFileOpen(file))
    // {
    //     print(Format("File content: {}\n", FileSystem::Read(file)).CStr());
    // }
=======
#if defined(RPP_USE_TEST)
    TestSystem::GetInstance()->Initialize(
        String("C:\\Users\\APC\\Projects\\robotics-programming-platform\\e2e-gruntime\\TestReports\\result.json"),
        String(""),
        String("C:\\Users\\APC\\Projects\\robotics-programming-platform\\e2e-gruntime\\empty_scenario.py"));
#endif
>>>>>>> 72dcf1c ([feautre] first simple e2e test with python)

    {
        CREATE_SESSION(TestSession, 800, 600, "Test2", TRUE);

        while (TRUE)
        {
#if defined(RPP_USE_TEST)
            if (!TestSystem::GetInstance()->ShouldApplicationClose())
            {
                TestSystem::GetInstance()->Update(0.0f);
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

    GraphicSessionManager::GetInstance()->ClearSessions();

    Signal::Shutdown();
    Thread::Shutdown();
    Renderer::Shutdown();
    FileSystem::Shutdown();
    SingletonManager::Shutdown();
    return 0;
}