#include "applications/applications.h"

RPP_ENABLE_MEMORY_TRACKING;

using namespace rpp;

const char *vertexShaderSource = R"(
#version 330 core

uniform float vScale;

layout(location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos.x * vScale, aPos.y * vScale, 0.0, 1.0);
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

const char *fragmentShaderSource2 = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
)";

int main(void)
{
    SingletonManager::Initialize();
    Logging::GetInstance()->Setup(u8(HandlerType::CONSOLE), LogLevel::DEBUG);
    Renderer::Initialize();

    u32 renderer1 = Renderer::CreateRenderer(800, 600, "Test");
    u32 renderer2 = Renderer::CreateRenderer(800, 600, "Test2");
    u32 renderer3 = Renderer::CreateRenderer(800, 600, "Test3");

    {
        HighResTimer timer;

        Renderer::ActivateRenderer(renderer1);
        u32 program = Program::Create(vertexShaderSource, fragmentShaderSource);

        u32 rectangle = Rectangle::Create();

        Renderer::ActivateRenderer(renderer2);
        u32 program2 = Program::Create(vertexShaderSource, fragmentShaderSource2);
        u32 rectangle2 = Rectangle::Create();

        Renderer::ActivateRenderer(renderer3);
        u32 program3 = Program::Create(vertexShaderSource, fragmentShaderSource2);
        u32 imgui = ImGuiImpl::Create();
        u32 line = Line::Create();

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

                    Program::Use(program);
                    Program::SetUniform("vScale", 0.5f);
                    Rectangle::Draw(rectangle, {-0.5f, -0.5f, 0.1f, 0.1f});

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

                    Program::Use(program2);
                    Program::SetUniform("vScale", 1.0f);
                    Rectangle::Draw(rectangle2, {-0.5f, -0.5f, 0.1f, 0.1f});

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

                    Program::Use(program3);
                    Program::SetUniform("vScale", 0.1f);
                    Line::Draw(line, {-0.5f, -0.5f}, {0.5f, 0.5f});

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