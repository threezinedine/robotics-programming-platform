#include "modules/renderer/line.h"
#include "modules/renderer/renderer_impl.h"

namespace rpp
{
    Scope<Storage<Line::LineData>> Line::s_lines = nullptr;

    void Line::Initialize()
    {
        RPP_ASSERT(s_lines == nullptr);

        s_lines = CreateScope<Storage<LineData>>();
    }

    void Line::Shutdown()
    {
        RPP_ASSERT(s_lines != nullptr);

        s_lines = nullptr;
    }

    u32 Line::Create()
    {
        RPP_ASSERT(s_lines != nullptr);

        u32 id = s_lines->Create();
        LineData *lineData = s_lines->Get(id);

        float data[] = {
            1.0f, 1.0f,  // Vertex 1 (X, Y)
            -1.0f, -1.0f // Vertex 2 (X, Y)
        };

        LayoutElement elements[] = {
            {AtomicType::FLOAT, 2} // Position attribute (vec2)
        };

        CreateVertexBufferCommandData command = {};
        command.count = 2;
        command.pData = data;
        command.layoutCount = 1;
        command.pLayout = elements;
        command.pBufferId = &lineData->vertexBufferId;
        command.pArrayId = &lineData->vertextArrayId;
        command.type = VertexBufferType::LINE;

        GraphicsCommandData commandData = {GraphicsCommandType::CREATE_VERTEX_BUFFER, &command};
        Renderer::GetWindow()->ExecuteCommand(commandData);

        return id;
    }

    void Line::Destroy(u32 id)
    {
        RPP_ASSERT(s_lines != nullptr);

        LineData *lineData = s_lines->Get(id);
        RPP_ASSERT(lineData != nullptr);

        DeleteVertexBufferCommandData command = {};
        command.pBufferId = &lineData->vertexBufferId;
        command.pArrayId = &lineData->vertextArrayId;
        GraphicsCommandData commandData = {GraphicsCommandType::DELETE_VERTEX_BUFFER, &command};
        Renderer::GetWindow()->ExecuteCommand(commandData);
    }

    void Line::Draw(u32 id, const Point &start, const Point &end)
    {
        RPP_ASSERT(s_lines != nullptr);
        LineData *lineData = s_lines->Get(id);
        RPP_ASSERT(lineData != nullptr);

        DrawVertexBufferCommandData command = {};
        command.bufferId = lineData->vertexBufferId;
        command.type = VertexBufferType::LINE;
        command.count = 2;

        GraphicsCommandData commandData = {GraphicsCommandType::DRAW_VERTEX_BUFFER, &command};
        Renderer::GetWindow()->ExecuteCommand(commandData);
    }
} // namespace rpp
