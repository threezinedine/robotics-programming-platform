#include "applications/applications.h"
#include "glm/gtc/matrix_transform.hpp"

RPP_ENABLE_MEMORY_TRACKING;

using namespace rpp;

int main(void)
{
    SingletonManager::Initialize();
    Logging::GetInstance()->Setup(u8(HandlerType::CONSOLE), LogLevel::DEBUG);
    Renderer::Initialize();

    u32 renderer1 = Renderer::Create(800, 600, "Test");
    u32 renderer2 = Renderer::Create(800, 600, "Test2");
    u32 renderer3 = Renderer::Create(800, 600, "Test3", TRUE);

    {
        HighResTimer timer;

        f32 delta = 0;

        while (TRUE)
        {
            timer.Start();

            b8 shouldApplicationClose = TRUE;

            if (renderer1 != INVALID_ID)
            {
                Renderer::Activate(renderer1);

                if (Renderer::GetWindow()->ShouldWindowClose())
                {
                    Renderer::Destroy(renderer1);
                    renderer1 = INVALID_ID;
                }
                else
                {
                    shouldApplicationClose = FALSE;
                    Renderer::PreDraw();

                    Renderer::DrawRectangle({-100, -100, 200, 200});

                    Renderer::PostDraw();

                    Renderer::Present();
                }
            }

            if (renderer2 != INVALID_ID)
            {
                Renderer::Activate(renderer2);

                if (Renderer::GetWindow()->ShouldWindowClose())
                {
                    Renderer::Destroy(renderer2);
                    renderer2 = INVALID_ID;
                }
                else
                {
                    shouldApplicationClose = FALSE;

                    Renderer::PreDraw();

                    Renderer::DrawRectangle({100, -100, 200, 200});

                    Renderer::PostDraw();

                    Renderer::Present();
                }
            }

            if (renderer3 != INVALID_ID)
            {
                Renderer::Activate(renderer3);

                if (Renderer::GetWindow()->ShouldWindowClose())
                {
                    Renderer::Destroy(renderer3);
                    renderer3 = INVALID_ID;
                }
                else
                {
                    shouldApplicationClose = FALSE;

                    Renderer::PreDraw();

                    Renderer::DrawRectangle({100, 100, 300, 400});

                    Renderer::PostDraw();

                    ImGui::Begin("Test Window");
                    ImGui::Text("Hello, world!");
                    ImGui::End();

                    ImGui::Begin("My Captured Scene Window");
                    Renderer::DrawingSceneInImGui();
                    ImGui::End();

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
        Renderer::Destroy(renderer1);
    }

    if (renderer2 != INVALID_ID)
    {
        Renderer::Destroy(renderer2);
    }

    if (renderer3 != INVALID_ID)
    {
        Renderer::Destroy(renderer3);
    }

    Renderer::Shutdown();
    SingletonManager::Shutdown();
    return 0;
}
