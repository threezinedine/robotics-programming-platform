#include "modules/renderer/rectangle.h"
#include "platforms/platforms.h"
#include "platforms/graphics/command.h"
#include "modules/renderer/renderer_impl.h"
#include "modules/renderer/program.h"
#include "glm/gtc/matrix_transform.hpp"
#include "modules/renderer/texture.h"

static const char *vertexShaderSource = R"(
#version 330 core

uniform float vScale;

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

void main()
{
    vTexCoord = aTexCoord;
    gl_Position = vec4(aPos.x * vScale, aPos.y * vScale, 0.0, 1.0);
}
)";

static const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

uniform sampler2D uTexture;

in vec2 vTexCoord;

void main()
{
	vec4 textureColor = texture(uTexture, vTexCoord);
    FragColor = vec4(1.0) * textureColor;
}
)";

namespace rpp
{
	Scope<Storage<Rectangle::RectangleData>> Rectangle::s_rectangles = nullptr;

	void Rectangle::Initialize()
	{
		RPP_LOG_DEBUG("Initializing Rectangle storage.");

		RPP_ASSERT(s_rectangles == nullptr);

		auto RectangleDeallocator = [&](Rectangle::RectangleData *data)
		{
			RPP_ASSERT(data != nullptr);
			RPP_ASSERT(data->rendererId != INVALID_ID);

			Renderer::ActivateRenderer(data->rendererId);

			DeleteVertexBufferCommandData command = {};
			command.pBufferId = &data->vertexBufferId;
			command.pArrayId = &data->vertexArrayId;
			GraphicsCommandData commandData = {GraphicsCommandType::DELETE_VERTEX_BUFFER, &command};
			Renderer::GetWindow()->ExecuteCommand(commandData);

			Program::Destroy(data->programId);

			RPP_DELETE(data);
		};

		s_rectangles = CreateScope<Storage<RectangleData>>(RectangleDeallocator);
	}

	void Rectangle::Shutdown()
	{
		RPP_LOG_DEBUG("Shutting down Rectangle storage.");

		RPP_ASSERT(s_rectangles != nullptr);

		s_rectangles.reset();
	}

	u32 Rectangle::Create()
	{
		RPP_LOG_DEBUG("Creating a new Rectangle instance");

		RPP_ASSERT(s_rectangles != nullptr);

		u32 program = Program::Create(vertexShaderSource, fragmentShaderSource);

		u32 id = s_rectangles->Create();
		RectangleData *rectangleData = s_rectangles->Get(id);
		rectangleData->rendererId = Renderer::GetCurrentRendererId();
		rectangleData->programId = program;

		float data[] = {
			-0.5f, -0.5f, 0.0f, 1.0f, // Bottom-left
			0.5f, -0.5f, 1.0f, 1.0f,  // Bottom-right
			0.5f, 0.5f, 1.0f, 0.0f,	  // Top-right
			0.5f, 0.5f, 1.0f, 0.0f,	  // Top-right
			-0.5f, 0.5f, 0.0f, 0.0f,  // Top-left
			-0.5f, -0.5f, 0.0f, 1.0f  // Bottom-left
		};

		LayoutElement elements[] = {
			{AtomicType::FLOAT, 2}, // Position attribute (vec3),
			{AtomicType::FLOAT, 2}, // Texture coordinate attribute (vec2)
		};

		CreateVertexBufferCommandData command = {};
		command.count = 6;
		command.pData = data;
		command.pBufferId = &rectangleData->vertexBufferId;
		command.pArrayId = &rectangleData->vertexArrayId;
		command.type = VertexBufferType::TRIANGLE;
		command.layoutCount = 2;
		command.pLayout = elements;

		GraphicsCommandData commandData = {GraphicsCommandType::CREATE_VERTEX_BUFFER, &command};
		Renderer::GetWindow()->ExecuteCommand(commandData);
		RPP_ASSERT(s_rectangles != nullptr);
		RPP_ASSERT(rectangleData->vertexBufferId != 0 && rectangleData->vertexArrayId != 0);

		return id;
	}

	void Rectangle::Destroy(u32 rectangleId)
	{
		RPP_LOG_DEBUG("Destroying Rectangle instance with ID: {}", rectangleId);
		RPP_ASSERT(s_rectangles != nullptr);

		s_rectangles->Free(rectangleId);
	}

	void Rectangle::Draw(u32 rectangleId, const Rect &rect, u32 textureId)
	{
		RPP_ASSERT(s_rectangles != nullptr);
		RectangleData *data = s_rectangles->Get(rectangleId);
		Program::Use(data->programId);
		Program::SetUniform("vScale", 0.5f);

		RPP_ASSERT(data->rendererId == Renderer::GetCurrentRendererId());

		if (textureId != INVALID_ID)
		{
			Texture::Activate(textureId, 0);
		}

		DrawVertexBufferCommandData command = {};
		command.bufferId = data->vertexArrayId;
		command.type = VertexBufferType::TRIANGLE;
		command.count = 6;

		GraphicsCommandData commandData = {GraphicsCommandType::DRAW_VERTEX_BUFFER, &command};
		Renderer::GetWindow()->ExecuteCommand(commandData);
	}
} // namespace rpp
