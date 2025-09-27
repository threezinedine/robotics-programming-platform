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
    Graphics::Init();

    {
		Renderer renderer(800, 600, "RPP Window");
		Program program(renderer, vertexShaderSource, fragmentShaderSource);

		Rectangle rectangle(renderer);

		while (!renderer.GetWindow()->ShouldWindowClose())
		{
			renderer.PreDraw();

			program.Use();
			rectangle.Draw();

			renderer.PostDraw();
			renderer.Present();
		}
    }

    Graphics::Shutdown();

    SingletonManager::Shutdown();
    return 0;
}