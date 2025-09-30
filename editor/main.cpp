#include "applications/applications.h"
#include "glm/gtc/matrix_transform.hpp"

RPP_ENABLE_MEMORY_TRACKING;

using namespace rpp;

int main(void)
{
    SingletonManager::Initialize();
    Logging::GetInstance()->Setup(u8(HandlerType::CONSOLE), LogLevel::DEBUG);
    Renderer::Initialize();

    u32 renderer1 = Renderer::CreateRenderer(800, 600, "Test");
    u32 renderer2 = Renderer::CreateRenderer(800, 600, "Test2");
    u32 renderer3 = Renderer::CreateRenderer(800, 600, "Test3");

    Mat4x4 move = glm::rotate(Mat4x4(1.0f), glm::radians(45.0f), Vec3(0.0f, 0.0f, 1.0f));

    {
        HighResTimer timer;

        Renderer::ActivateRenderer(renderer3);
        u32 imgui = ImGuiImpl::Create();

        f32 delta = 0;

        while (TRUE)
        {
            timer.Start();

            b8 shouldApplicationClose = TRUE;

            if (renderer1 != INVALID_ID)
            {
                Renderer::ActivateRenderer(renderer1);

                if (Renderer::GetWindow()->ShouldWindowClose())
                {
                    Renderer::DestroyRenderer(renderer1);
                    renderer1 = INVALID_ID;
                }
                else
                {
                    shouldApplicationClose = FALSE;
                    Renderer::PreDraw();

                    Renderer::DrawRectangle({-0.5f, -0.5f, 0.1f, 0.1f});

                    Renderer::PostDraw();

                    Renderer::Present();
                }
            }

            if (renderer2 != INVALID_ID)
            {
                Renderer::ActivateRenderer(renderer2);

                if (Renderer::GetWindow()->ShouldWindowClose())
                {
                    Renderer::DestroyRenderer(renderer2);
                    renderer2 = INVALID_ID;
                }
                else
                {
                    shouldApplicationClose = FALSE;

                    Renderer::PreDraw();

                    Renderer::DrawRectangle({-0.5f, -0.5f, 0.1f, 0.1f});

                    Renderer::PostDraw();

                    Renderer::Present();
                }
            }

            if (renderer3 != INVALID_ID)
            {
                Renderer::ActivateRenderer(renderer3);

                if (Renderer::GetWindow()->ShouldWindowClose())
                {
                    Renderer::DestroyRenderer(renderer3);
                    renderer3 = INVALID_ID;
                }
                else
                {
                    shouldApplicationClose = FALSE;

                    ImGuiImpl::PrepareFrame(imgui);
                    Renderer::PreDraw();

                    Renderer::DrawLine({-0.5f, -0.5f}, {0.5f, 0.5f});

                    Renderer::PostDraw();

                    ImGui::Begin("Test Window");
                    ImGui::Text("Hello, world!");
                    ImGui::End();

                    ImGuiImpl::Render(imgui);

                    Renderer::Present();
                }
            }

            if (shouldApplicationClose)
            {
                break;
            }
        }
    }

    if (renderer1 != INVALID_ID)
    {
        Renderer::DestroyRenderer(renderer1);
    }

    if (renderer2 != INVALID_ID)
    {
        Renderer::DestroyRenderer(renderer2);
    }

    if (renderer3 != INVALID_ID)
    {
        Renderer::DestroyRenderer(renderer3);
    }

    Renderer::Shutdown();
    SingletonManager::Shutdown();
    return 0;
}
