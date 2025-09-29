#include "applications/applications.h"
#include "glm/gtc/matrix_transform.hpp"

RPP_ENABLE_MEMORY_TRACKING;

using namespace rpp;

const char *vertexShaderSource = R"(
#version 330 core

uniform float vScale;
uniform mat4 rotateMat;

layout(location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos.x * vScale, aPos.y * vScale, 0.0, 1.0);
    gl_Position = rotateMat * gl_Position;
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

    {
        u32 renderer = Renderer::CreateRenderer(800, 600, "Test");
        Mat4x4 move = glm::rotate(Mat4x4(1.0f), glm::radians(45.0f), Vec3(0.0f, 0.0f, 1.0f));

        Renderer::ActivateRenderer(renderer);
        u32 program = Program::Create(vertexShaderSource, fragmentShaderSource);
        u32 rectangle = Rectangle::Create();
        u32 imgui = ImGuiImpl::Create();

        while (TRUE)
        {
            Renderer::ActivateRenderer(renderer);

            if (Renderer::GetWindow()->ShouldWindowClose())
            {
                Renderer::DestroyRenderer(renderer);
                break;
            }
            else
            {
                ImGuiImpl::PrepareFrame(imgui);

                u32 program = Program::Create(vertexShaderSource, fragmentShaderSource);
                u32 rectangle = Rectangle::Create();

                Renderer::PreDraw();
                Program::Use(program);
                Program::SetUniform("vScale", 0.5f);
                Program::SetUniform("rotateMat", move);
                Rectangle::Draw(rectangle, {-0.5f, -0.5f, 0.1f, 0.1f});

                Renderer::PostDraw();

                ImGui::ShowDemoWindow();

                ImGuiImpl::Render(imgui);

                Renderer::Present();
            }
        }
    }

    Renderer::Shutdown();
    SingletonManager::Shutdown();
    return 0;
}