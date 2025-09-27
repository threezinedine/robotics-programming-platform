#include "modules/renderer/program.h"
#include "modules/renderer/renderer_impl.h"

namespace rpp
{
    Program::Program()
        : m_programId(0)
    {
    }

    Program::Program(const String &vertexShaderSource, const String &fragmentShaderSource)
        : m_programId(0)
    {
        u32 vertexShaderId = 0;

        CreateShaderCommandData vertexShaderCommand = {};
        vertexShaderCommand.shaderSource = (u8 *)vertexShaderSource.CStr();
        vertexShaderCommand.length = vertexShaderSource.Length();
        vertexShaderCommand.type = ShaderType::VERTEX;
        vertexShaderCommand.shaderId = &vertexShaderId;

        GraphicsCommandData vertexShaderCommandData = {GraphicsCommandType::CREATE_SHADER, &vertexShaderCommand};
        Renderer::GetCurrentRenderer().GetWindow()->ExecuteCommand(vertexShaderCommandData);

        u32 fragmentShaderId = 0;
        CreateShaderCommandData fragmentShaderCommand = {};
        fragmentShaderCommand.shaderSource = (u8 *)fragmentShaderSource.CStr();
        fragmentShaderCommand.length = fragmentShaderSource.Length();
        fragmentShaderCommand.type = ShaderType::FRAGMENT;
        fragmentShaderCommand.shaderId = &fragmentShaderId;

        GraphicsCommandData fragmentShaderCommandData = {GraphicsCommandType::CREATE_SHADER, &fragmentShaderCommand};
        Renderer::GetCurrentRenderer().GetWindow()->ExecuteCommand(fragmentShaderCommandData);

        CreatePipelineCommandData pipelineCommand = {};
        pipelineCommand.vertexShaderId = vertexShaderId;
        pipelineCommand.fragmentShaderId = fragmentShaderId;
        pipelineCommand.programId = &m_programId;

        GraphicsCommandData pipelineCommandData = {GraphicsCommandType::CREATE_PIPELINE, &pipelineCommand};
        Renderer::GetCurrentRenderer().GetWindow()->ExecuteCommand(pipelineCommandData);

        // After creating the pipeline, we can delete the individual shaders
        DeleteShaderCommandData deleteVertexShaderCommand = {};
        deleteVertexShaderCommand.shaderId = vertexShaderId;

        GraphicsCommandData deleteVertexShaderCommandData = {GraphicsCommandType::DELETE_SHADER, &deleteVertexShaderCommand};
        Renderer::GetCurrentRenderer().GetWindow()->ExecuteCommand(deleteVertexShaderCommandData);

        DeleteShaderCommandData deleteFragmentShaderCommand = {};
        deleteFragmentShaderCommand.shaderId = fragmentShaderId;

        GraphicsCommandData deleteFragmentShaderCommandData = {GraphicsCommandType::DELETE_SHADER, &deleteFragmentShaderCommand};
        Renderer::GetCurrentRenderer().GetWindow()->ExecuteCommand(deleteFragmentShaderCommandData);
    }

    Program::~Program()
    {
    }

    void Program::Use() const
    {
        UsePipelineCommandData useCommand = {};
        useCommand.programId = m_programId;

        GraphicsCommandData useCommandData = {GraphicsCommandType::USE_PIPELINE, &useCommand};
        Renderer::GetCurrentRenderer().GetWindow()->ExecuteCommand(useCommandData);
    }
} // namespace rpp
