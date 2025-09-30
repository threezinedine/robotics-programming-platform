#include "modules/renderer/line.h"
#include "modules/renderer/renderer_impl.h"
#include "modules/renderer/program.h"

static const char *vertexShaderSource = R"(
#version 330 core

uniform float vScale;

layout(location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos.x * vScale, aPos.y * vScale, 0.0, 1.0);
}
)";

static const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0);
}
)";

namespace rpp
{
    Scope<Storage<Line::LineData>> Line::s_lines = nullptr;

    void Line::Initialize()
    {
        RPP_ASSERT(s_lines == nullptr);

        auto LineDeallocator = [](LineData *data)
        {
            RPP_ASSERT(data != nullptr);

            Program::Destroy(data->programId);

            DeleteVertexBufferCommandData command = {};
            command.pBufferId = &data->vertexBufferId;
            command.pArrayId = &data->vertexArrayId;
            GraphicsCommandData commandData = {GraphicsCommandType::DELETE_VERTEX_BUFFER, &command};
            Renderer::GetWindow()->ExecuteCommand(commandData);

            RPP_DELETE(data);
        };

        s_lines = CreateScope<Storage<LineData>>(LineDeallocator);
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

        lineData->rendererId = Renderer::GetCurrentRendererId();
        lineData->programId = Program::Create(vertexShaderSource, fragmentShaderSource);

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
        command.pArrayId = &lineData->vertexArrayId;
        command.type = VertexBufferType::LINE;

        GraphicsCommandData commandData = {GraphicsCommandType::CREATE_VERTEX_BUFFER, &command};
        Renderer::GetWindow()->ExecuteCommand(commandData);

        return id;
    }

    void Line::Destroy(u32 id)
    {
        RPP_ASSERT(s_lines != nullptr);
        s_lines->Free(id);
    }

    void Line::Draw(u32 id, const Point &start, const Point &end)
    {
        RPP_ASSERT(s_lines != nullptr);
        LineData *lineData = s_lines->Get(id);
        RPP_ASSERT(lineData != nullptr);
        RPP_ASSERT(lineData->rendererId == Renderer::GetCurrentRendererId());

        Program::Use(lineData->programId);
        Program::SetUniform("vScale", 1.0f);

        DrawVertexBufferCommandData command = {};
        command.bufferId = lineData->vertexBufferId;
        command.type = VertexBufferType::LINE;
        command.count = 2;

        GraphicsCommandData commandData = {GraphicsCommandType::DRAW_VERTEX_BUFFER, &command};
        Renderer::GetWindow()->ExecuteCommand(commandData);
    }
} // namespace rpp
