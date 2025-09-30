#include "applications/applications.h"
#include "glm/gtc/matrix_transform.hpp"

RPP_ENABLE_MEMORY_TRACKING;

using namespace rpp;

int main(void)
{
    SingletonManager::Initialize();
    Logging::GetInstance()->Setup(u8(HandlerType::CONSOLE), LogLevel::DEBUG);
    Renderer::Initialize();

    {
        u32 renderer = Renderer::CreateRenderer(800, 600, "Test");

        Renderer::ActivateRenderer(renderer);
        u32 imgui = ImGuiImpl::Create();
        // u32 texture = Texture::Create("C:\\Users\\APC\\Downloads\\download.jpg");
        u32 texture = Texture::Create("C:\\Users\\APC\\Pictures\\Screenshots\\Screenshot 2025-08-19 222422.png");

        while (TRUE)
        {
            Renderer::ActivateRenderer(renderer);

            if (Renderer::GetWindow()->ShouldWindowClose())
            {
                ImGuiImpl::Destroy(imgui);
                Texture::Destroy(texture);
                Renderer::DestroyRenderer(renderer);
                break;
            }
            else
            {
                // ImGuiImpl::PrepareFrame(imgui);

                Renderer::PreDraw();

                Renderer::DrawRectangle({-0.5f, -0.5f, 0.1f, 0.1f}, texture);
                Renderer::DrawLine({-0.5f, -0.5f}, {0.5f, 0.5f});

                Renderer::PostDraw();

                // ImGui::ShowDemoWindow();

                // ImGui::Begin("My Captured Scene Window");
                // ImGuiImpl::DrawRenderingScene(imgui);
                // ImGui::End();

                // ImGuiImpl::Render(imgui);

                Renderer::Present();
            }
        }
    }

    Renderer::Shutdown();
    SingletonManager::Shutdown();
    return 0;
}