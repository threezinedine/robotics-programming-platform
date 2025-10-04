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

void SortingTestThread(void *param)
{
    i32 *pElement = static_cast<i32 *>(param);

    Thread::Sleep(*pElement * 10); // sleep for element * 10 milliseconds

    print(Format("{}, ", *pElement).CStr(), ConsoleColor::GREEN);
}

int main(void)
{
    SingletonManager::Initialize();
    FileSystem::Initialize();
    Logging::GetInstance()->Setup(u8(HandlerType::CONSOLE), LogLevel::DEBUG);
    Renderer::Initialize();
    Thread::Initialize();

    i32 array[] = {2, 1, 4, 2, 5};
    Array<ThreadId> threads;

    // thread sorting for testing purpose
    for (u32 i = 0; i < sizeof(array) / sizeof(i32); i++)
    {
        ThreadId threadId = Thread::Create(SortingTestThread, &array[i], sizeof(i32));
        threads.Push(threadId);
    }

    u32 threadCount = threads.Size();
    for (u32 i = 0; i < threadCount; i++)
    {
        Thread::Start(threads[i]);
    }

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

    Thread::Shutdown();
    Renderer::Shutdown();
    FileSystem::Shutdown();
    SingletonManager::Shutdown();
    return 0;
}