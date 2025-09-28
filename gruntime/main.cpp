#include "applications/applications.h"

RPP_ENABLE_MEMORY_TRACKING;

using namespace rpp;

const char *vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0);
}
)";

int main(void)
{
    SingletonManager::Initialize();
    Logging::GetInstance()->Setup(u8(HandlerType::CONSOLE), LogLevel::DEBUG);
    Renderer::Initialize();

    u32 renderer1 = Renderer::CreateRenderer(800, 600, "Test");
    u32 renderer2 = Renderer::CreateRenderer(800, 600, "Test2");

    {
        HighResTimer timer;

        Renderer::ActivateRenderer(renderer1);

        u32 program = Program::Create(vertexShaderSource, fragmentShaderSource);

        u32 rectangle = Rectangle::Create();
        f32 delta = 0;

        while (TRUE)
        {
            timer.Start();

            b8 shouldRender1Close = FALSE;
            b8 shouldRender2Close = FALSE;

            if (renderer1 == INVALID_ID && renderer2 == INVALID_ID)
            {
                break;
            }

            if (renderer1 != INVALID_ID)
            {
                Renderer::ActivateRenderer(renderer1);
                shouldRender1Close = Renderer::GetWindow()->ShouldWindowClose();
            }

            if (renderer2 != INVALID_ID)
            {
                Renderer::ActivateRenderer(renderer2);
                shouldRender2Close = Renderer::GetWindow()->ShouldWindowClose();
            }

            if (shouldRender1Close && renderer1 != INVALID_ID)
            {
                Renderer::DestroyRenderer(renderer1);
                renderer1 = INVALID_ID;
            }

            if (shouldRender2Close && renderer2 != INVALID_ID)
            {
                Renderer::DestroyRenderer(renderer2);
                renderer2 = INVALID_ID;
            }

            if (renderer1 != INVALID_ID)
            {
                Renderer::ActivateRenderer(renderer1);
                Renderer::PreDraw();

                Program::Use(program);

                Rectangle::Draw(rectangle, {-0.5f, -0.5f, 0.1f, 0.1f});

                Renderer::PostDraw();

                Renderer::Present();
            }

            if (renderer2 != INVALID_ID)
            {
                Renderer::ActivateRenderer(renderer2);
                Renderer::PreDraw();

                Renderer::PostDraw();

                Renderer::Present();
            }

            delta = (f32)timer.GetElapsedTimeInMilliseconds();
            RPP_LOG_INFO("Frame Time: {} ms", delta);
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

    Renderer::Shutdown();
    SingletonManager::Shutdown();
    return 0;
}