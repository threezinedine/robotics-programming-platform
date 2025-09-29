#include "modules/renderer/rectangle.h"
#include "platforms/platforms.h"
#include "platforms/graphics/command.h"
#include "modules/renderer/renderer_impl.h"
#include "modules/renderer/program.h"
#include "glm/gtc/matrix_transform.hpp"

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

namespace rpp
{
	Scope<Storage<Rectangle::RectangleData>> Rectangle::s_rectangles = nullptr;

	void Rectangle::Initialize()
	{
		RPP_ASSERT(s_rectangles == nullptr);

		s_rectangles = CreateScope<Storage<RectangleData>>();
	}

	void Rectangle::Shutdown()
	{
		RPP_ASSERT(s_rectangles != nullptr);

		s_rectangles.reset();
	}

	u32 Rectangle::Create()
	{
		u32 program = Program::Create(vertexShaderSource, fragmentShaderSource);

		u32 id = s_rectangles->Create();
		RectangleData *rectangleData = s_rectangles->Get(id);
		rectangleData->rendererId = Renderer::GetCurrentRendererId();
		rectangleData->programId = program;

		float data[] = {
			-0.5f, -0.5f, // Bottom-left
			0.5f, -0.5f,  // Bottom-right
			0.5f, 0.5f,	  // Top-right
			0.5f, 0.5f,	  // Top-right
			-0.5f, 0.5f,  // Top-left
			-0.5f, -0.5f  // Bottom-left
		};

		LayoutElement elements[] = {
			{AtomicType::FLOAT, 2} // Position attribute (vec3),
		};

		CreateVertexBufferCommandData command = {};
		command.count = 6;
		command.pData = data;
		command.pBufferId = &rectangleData->vertexBufferId;
		command.pArrayId = &rectangleData->vertexArrayId;
		command.type = VertexBufferType::TRIANGLE;
		command.layoutCount = 1;
		command.pLayout = elements;

		GraphicsCommandData commandData = {GraphicsCommandType::CREATE_VERTEX_BUFFER, &command};
		Renderer::GetWindow()->ExecuteCommand(commandData);
		RPP_ASSERT(s_rectangles != nullptr);
		RPP_ASSERT(rectangleData->vertexBufferId != 0 && rectangleData->vertexArrayId != 0);

		return id;
	}

	void Rectangle::Destroy(u32 rectangleId)
	{
		RPP_ASSERT(s_rectangles != nullptr);
		RectangleData *data = s_rectangles->Get(rectangleId);

		RPP_ASSERT(data->rendererId == Renderer::GetCurrentRendererId());

		DeleteVertexBufferCommandData command = {};
		command.pBufferId = &data->vertexBufferId;
		command.pArrayId = &data->vertexArrayId;

		GraphicsCommandData commandData = {GraphicsCommandType::DELETE_VERTEX_BUFFER, &command};
		Renderer::GetWindow()->ExecuteCommand(commandData);

		s_rectangles->Free(rectangleId);
	}

	void Rectangle::Draw(u32 rectangleId, const Rect &rect)
	{
		RPP_ASSERT(s_rectangles != nullptr);
		RectangleData *data = s_rectangles->Get(rectangleId);
		Program::Use(data->programId);
		Program::SetUniform("vScale", 0.5f);
		Mat4x4 move = glm::rotate(Mat4x4(1.0f), glm::radians(45.0f), Vec3(0.0f, 0.0f, 1.0f));
		Program::SetUniform("rotateMat", move);

		RPP_ASSERT(data->rendererId == Renderer::GetCurrentRendererId());

		DrawVertexBufferCommandData command = {};
		command.bufferId = data->vertexArrayId;
		command.type = VertexBufferType::TRIANGLE;
		command.count = 6;

		GraphicsCommandData commandData = {GraphicsCommandType::DRAW_VERTEX_BUFFER, &command};
		Renderer::GetWindow()->ExecuteCommand(commandData);
	}
} // namespace rpp
