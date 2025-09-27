#include "modules/renderer/rectangle.h"
#include "platforms/platforms.h"
#include "platforms/graphics/command.h"
#include "modules/renderer/renderer_impl.h"

namespace rpp
{
	Rectangle::Rectangle()
		: Rectangle(0.0f, 0.0f, 1.0f, 1.0f)
	{
	}

	Rectangle::Rectangle(f32 centerX, f32 centerY, f32 width, f32 height)
		: m_centerX(centerX), m_centerY(centerY), m_width(width), m_height(height),
		  m_vertexBufferId(0), m_vertexArrayId(0)
	{
		float data[] = {
			centerX - width / 2, centerY - height / 2, // Bottom-left
			centerX + width / 2, centerY - height / 2, // Bottom-right
			centerX + width / 2, centerY + height / 2, // Top-right

			centerX + width / 2, centerY + height / 2, // Top-right
			centerX - width / 2, centerY + height / 2, // Top-left
			centerX - width / 2, centerY - height / 2  // Bottom-left
		};

		LayoutElement elements[] = {
			{AtomicType::FLOAT, 2} // Position attribute (vec3),
		};

		CreateVertexBufferCommandData command = {};
		command.count = 6;
		command.pData = data;
		command.pBufferId = &m_vertexBufferId;
		command.pArrayId = &m_vertexArrayId;
		command.type = VertexBufferType::TRIANGLE;
		command.layoutCount = 1;
		command.pLayout = elements;

		GraphicsCommandData commandData = {GraphicsCommandType::CREATE_VERTEX_BUFFER, &command};
		Renderer::GetCurrentRenderer().GetWindow()->ExecuteCommand(commandData);
	}

	Rectangle::~Rectangle()
	{
		if (m_vertexBufferId != 0)
		{
			DeleteVertexBufferCommandData command = {};
			command.pBufferId = &m_vertexBufferId;
			command.pArrayId = &m_vertexArrayId;

			GraphicsCommandData commandData = {GraphicsCommandType::DELETE_VERTEX_BUFFER, &command};
			Renderer::GetCurrentRenderer().GetWindow()->ExecuteCommand(commandData);
		}
	}

	void Rectangle::Draw() const
	{
		DrawVertexBufferCommandData command = {};
		command.bufferId = m_vertexArrayId;
		command.type = VertexBufferType::TRIANGLE;
		command.count = 6;

		GraphicsCommandData commandData = {GraphicsCommandType::DRAW_VERTEX_BUFFER, &command};
		Renderer::GetCurrentRenderer().GetWindow()->ExecuteCommand(commandData);
	}
} // namespace rpp
