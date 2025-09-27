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

    {
        Renderer renderer(800, 600, "RPP Window");

        renderer.Active();

        Program program(vertexShaderSource, fragmentShaderSource);

        Rectangle rectangle(0.6f, 0.0f, 0.1f, 0.1f);
        Line line({-0.5f, -0.5f}, {0.5f, 0.5f});

        while (!renderer.GetWindow()->ShouldWindowClose())
        {
            renderer.PreDraw();

            program.Use();
            rectangle.Draw();

            line.Draw();

            renderer.PostDraw();
            renderer.Present();
        }
    }

    Renderer::Shutdown();
    SingletonManager::Shutdown();
    return 0;
}