#include "applications/applications.h"
#include "glm/gtc/matrix_transform.hpp"

RPP_ENABLE_MEMORY_TRACKING;

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
        m_texture = Texture::Create("C:\\Users\\APC\\Pictures\\Screenshots\\Screenshot 2025-08-19 222422.png");
        RPP_ASSERT(m_texture != INVALID_ID);
    }

    void RenderImpl() override
    {
        RPP_ASSERT(m_texture != INVALID_ID);

        Renderer::DrawRectangle({0, 0, 100, 100}, m_texture);
        Renderer::DrawCircle({50, 50}, 50, m_texture);

        ImGui::ShowDemoWindow();

        ImGui::Begin("My Captured Scene Window");
        Renderer::DrawingSceneInImGui();
        ImGui::End();
    }

    void ShutdownImpl() override
    {
        RPP_ASSERT(m_texture != INVALID_ID);

        Texture::Destroy(m_texture);
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

    FileHandle file = FileSystem::OpenFile("C:\\Users\\APC\\Downloads\\test.txt");
    RPP_ASSERT(FileSystem::IsFileOpen(file));
    print(Format("File content: {}\n", FileSystem::Read(file)).CStr());
    FileSystem::CloseFile(file);

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

    Renderer::Shutdown();
    FileSystem::Shutdown();
    SingletonManager::Shutdown();
    return 0;
}