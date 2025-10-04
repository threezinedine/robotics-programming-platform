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

struct SignalTestParam
{
    SignalId mainSignal;
    SignalId threadSignal;
};

void SignalTestThread(void *pParam)
{
    SignalTestParam *testParam = static_cast<SignalTestParam *>(pParam);
    SignalId signal = testParam->threadSignal;
    SignalId mainSignal = testParam->mainSignal;

    print("Push 3\n", ConsoleColor::YELLOW);
    Signal::Active(mainSignal); // Notify main thread

    Signal::Wait(signal);       // Wait for the signal to be activated
    print("Push 5\n", ConsoleColor::YELLOW);
    Signal::Active(mainSignal); // Notify main thread

    Signal::Wait(signal); // Wait for the signal to be activated
    print("Push 7\n", ConsoleColor::YELLOW);
    Signal::Active(mainSignal); // Notify main thread
}

int main(void)
{
    SingletonManager::Initialize();
    FileSystem::Initialize();
    Logging::GetInstance()->Setup(u8(HandlerType::CONSOLE), LogLevel::DEBUG);
    Renderer::Initialize();
    Thread::Initialize();
    Signal::Initialize();

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

    {
        CREATE_SESSION(TestSession, 800, 600, "Test2", TRUE);

        while (TRUE)
        {
            if (GraphicSessionManager::GetInstance()->Update(0.0f))
            {
                break;
            }
        }
    }

    GraphicSessionManager::GetInstance()->ClearSessions();

    Signal::Shutdown();
    Thread::Shutdown();
    Renderer::Shutdown();
    FileSystem::Shutdown();
    SingletonManager::Shutdown();
    return 0;
}