#include "modules/renderer/rectangle.h"
#include "platforms/platforms.h"
#include "platforms/graphics/command.h"
#include "modules/renderer/renderer_impl.h"

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
		u32 id = s_rectangles->Create();
		RectangleData *rectangleData = s_rectangles->Get(id);
		rectangleData->rendererId = Renderer::GetCurrentRendererId();

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

		RPP_ASSERT(data->rendererId == Renderer::GetCurrentRendererId());

		DrawVertexBufferCommandData command = {};
		command.bufferId = data->vertexArrayId;
		command.type = VertexBufferType::TRIANGLE;
		command.count = 6;

		GraphicsCommandData commandData = {GraphicsCommandType::DRAW_VERTEX_BUFFER, &command};
		Renderer::GetWindow()->ExecuteCommand(commandData);
	}
} // namespace rpp
