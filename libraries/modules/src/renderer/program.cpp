#include "modules/renderer/program.h"
#include "modules/renderer/renderer_impl.h"

namespace rpp
{
    Scope<Storage<Program::ProgramData>> Program::s_programs = nullptr;
    u32 Program::s_currentProgramId = INVALID_ID;

    void Program::Initialize()
    {
        RPP_ASSERT(s_programs == nullptr);

        s_programs = CreateScope<Storage<ProgramData>>();
    }

    void Program::Shutdown()
    {
        RPP_ASSERT(s_programs != nullptr);

        s_programs.reset();
    }

    u32 Program::Create(const String &vertexShaderSource, const String &fragmentShaderSource)
    {
        RPP_ASSERT(s_programs != nullptr);

        u32 programId = s_programs->Create();
        ProgramData *data = s_programs->Get(programId);

        u32 vertexShaderId = 0;

        CreateShaderCommandData vertexShaderCommand = {};
        vertexShaderCommand.pShaderSource = (u8 *)vertexShaderSource.CStr();
        vertexShaderCommand.length = vertexShaderSource.Length();
        vertexShaderCommand.type = ShaderType::VERTEX;
        vertexShaderCommand.pShaderId = &vertexShaderId;

        GraphicsCommandData vertexShaderCommandData = {GraphicsCommandType::CREATE_SHADER, &vertexShaderCommand};
        Renderer::GetWindow()->ExecuteCommand(vertexShaderCommandData);

        u32 fragmentShaderId = 0;
        CreateShaderCommandData fragmentShaderCommand = {};
        fragmentShaderCommand.pShaderSource = (u8 *)fragmentShaderSource.CStr();
        fragmentShaderCommand.length = fragmentShaderSource.Length();
        fragmentShaderCommand.type = ShaderType::FRAGMENT;
        fragmentShaderCommand.pShaderId = &fragmentShaderId;

        GraphicsCommandData fragmentShaderCommandData = {GraphicsCommandType::CREATE_SHADER, &fragmentShaderCommand};
        Renderer::GetWindow()->ExecuteCommand(fragmentShaderCommandData);

        CreatePipelineCommandData pipelineCommand = {};
        pipelineCommand.vertexShaderId = vertexShaderId;
        pipelineCommand.fragmentShaderId = fragmentShaderId;
        pipelineCommand.pProgramId = &data->programId;

        GraphicsCommandData pipelineCommandData = {GraphicsCommandType::CREATE_PIPELINE, &pipelineCommand};
        Renderer::GetWindow()->ExecuteCommand(pipelineCommandData);

        // After creating the pipeline, we can delete the individual shaders
        DeleteShaderCommandData deleteVertexShaderCommand = {};
        deleteVertexShaderCommand.shaderId = vertexShaderId;

        GraphicsCommandData deleteVertexShaderCommandData = {GraphicsCommandType::DELETE_SHADER, &deleteVertexShaderCommand};
        Renderer::GetWindow()->ExecuteCommand(deleteVertexShaderCommandData);

        DeleteShaderCommandData deleteFragmentShaderCommand = {};
        deleteFragmentShaderCommand.shaderId = fragmentShaderId;

        GraphicsCommandData deleteFragmentShaderCommandData = {GraphicsCommandType::DELETE_SHADER, &deleteFragmentShaderCommand};
        Renderer::GetWindow()->ExecuteCommand(deleteFragmentShaderCommandData);

        return programId;
    }

    void Program::Destroy(u32 programId)
    {
        RPP_ASSERT(s_programs != nullptr);

        ProgramData *data = s_programs->Get(programId);

        DeletePipelineCommandData deleteCommand = {};
        deleteCommand.programId = data->programId;
        GraphicsCommandData deleteCommandData = {GraphicsCommandType::DELETE_PIPELINE, &deleteCommand};
        Renderer::GetWindow()->ExecuteCommand(deleteCommandData);

        s_programs->Free(programId);
    }

    void Program::Use(u32 programId)
    {
        RPP_ASSERT(s_programs != nullptr);

        ProgramData *data = s_programs->Get(programId);

        UsePipelineCommandData useCommand = {};
        useCommand.programId = data->programId;

        GraphicsCommandData useCommandData = {GraphicsCommandType::USE_PIPELINE, &useCommand};
        Renderer::GetWindow()->ExecuteCommand(useCommandData);

		s_currentProgramId = data->programId;
    }

    void Program::SetUniform(const String &name, f32 value)
    {
        RPP_ASSERT(s_programs != nullptr);
        RPP_ASSERT(s_currentProgramId != INVALID_ID); // Ensure a program is currently active

        SetUniformCommandData command = {};
        UniformDescription uniform = {};
        uniform.name = name.CStr();
        uniform.type = UniformType::FLOAT;
        uniform.pData = &value;

        command.uniformCount = 1;
        command.programId = s_currentProgramId;
        command.pUniforms = &uniform;

        GraphicsCommandData commandData = {GraphicsCommandType::SET_UNIFORM, &command};
        Renderer::GetWindow()->ExecuteCommand(commandData);
    }
} // namespace rpp
