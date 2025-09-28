#include "modules/renderer/line.h"
#include "modules/renderer/renderer_impl.h"

namespace rpp
{
    Line::Line()
        : Line({0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}) // Default line from (0,0) to (1,1) with white color
    {
    }

    Line::Line(const Point &start, const Point &end)
        : Line(start, end, {1.0f, 1.0f, 1.0f, 1.0f}) // Default color is white
    {
    }

    Line::Line(const Point &start, const Point &end, const Color &color)
    {
        float data[] = {
            start.x, start.y, // Start point
            end.x, end.y      // End point
        };

        LayoutElement elements[] = {
            {AtomicType::FLOAT, 2} // Position attribute (vec2)
        };

        CreateVertexBufferCommandData command = {};
        command.count = 2;
        command.pData = data;
        command.layoutCount = 1;
        command.pLayout = elements;
        command.pBufferId = &m_vertexBufferId;
        command.pArrayId = &m_vertexArrayId;
        command.type = VertexBufferType::LINE;

        GraphicsCommandData commandData = {GraphicsCommandType::CREATE_VERTEX_BUFFER, &command};
        Renderer::GetWindow()->ExecuteCommand(commandData);
    }

    Line::~Line()
    {
        DeleteVertexBufferCommandData command = {};
        command.pBufferId = &m_vertexBufferId;
        command.pArrayId = &m_vertexArrayId;
        GraphicsCommandData commandData = {GraphicsCommandType::DELETE_VERTEX_BUFFER, &command};
        Renderer::GetWindow()->ExecuteCommand(commandData);
    }

    void Line::Draw() const
    {
        DrawVertexBufferCommandData command = {};
        command.bufferId = m_vertexArrayId;
        command.type = VertexBufferType::LINE;
        command.count = 2;

        GraphicsCommandData commandData = {GraphicsCommandType::DRAW_VERTEX_BUFFER, &command};
        Renderer::GetWindow()->ExecuteCommand(commandData);
    }
} // namespace rpp
