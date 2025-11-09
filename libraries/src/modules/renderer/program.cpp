#include "modules/renderer/program.h"
#include "modules/renderer/renderer_impl.h"
#include "modules/renderer/type.h"

namespace rpp
{
    Scope<Storage<Program::ProgramData>> Program::s_programs = nullptr;
    u32 Program::s_currentProgramId = INVALID_ID;

    void Program::Initialize()
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_programs == nullptr);

        auto ProgramDeallocator = [](Program::ProgramData *data)
        {
            RPP_ASSERT(data != nullptr);
            RPP_ASSERT(data->rendererId != INVALID_ID);

            DeletePipelineCommandData deleteCommand = {};
            deleteCommand.programId = data->programId;
            GraphicsCommandData deleteCommandData = {GraphicsCommandType::DELETE_PIPELINE, &deleteCommand};
            Renderer::GetWindow()->ExecuteCommand(deleteCommandData);

            RPP_DELETE(data);
        };

        s_programs = CreateScope<Storage<ProgramData>>(ProgramDeallocator);
    }

    void Program::Shutdown()
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_programs != nullptr);

        s_programs.reset();
    }

    u32 Program::Create(const String &vertexShaderSource, const String &fragmentShaderSource)
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_programs != nullptr);

        u32 programId = s_programs->Create();
        ProgramData *data = s_programs->Get(programId);
        data->rendererId = Renderer::GetCurrentRendererId();

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
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_programs != nullptr);
        s_programs->Free(programId);
    }

    void Program::Use(u32 programId)
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(s_programs != nullptr);

        ProgramData *data = s_programs->Get(programId);

        UsePipelineCommandData useCommand = {};
        useCommand.programId = data->programId;

        GraphicsCommandData useCommandData = {GraphicsCommandType::USE_PIPELINE, &useCommand};
        Renderer::GetWindow()->ExecuteCommand(useCommandData);

        s_currentProgramId = programId;
    }

#define DEFINE_SET_UNIFORM(valueType, uniformType)                                      \
    template <>                                                                         \
    void Program::SetUniform<valueType>(const String &name, valueType value)            \
    {                                                                                   \
        RPP_PROFILE_SCOPE();                                                            \
        RPP_ASSERT(s_programs != nullptr);                                              \
        RPP_ASSERT(s_currentProgramId != INVALID_ID);                                   \
        ProgramData *data = s_programs->Get(s_currentProgramId);                        \
        RPP_ASSERT(data != nullptr);                                                    \
                                                                                        \
        SetUniformCommandData command = {};                                             \
        UniformDescription uniform = {};                                                \
        uniform.name = name.CStr();                                                     \
        uniform.type = uniformType;                                                     \
        uniform.pData = &value;                                                         \
                                                                                        \
        command.uniformCount = 1;                                                       \
        command.programId = data->programId;                                            \
        command.pUniforms = &uniform;                                                   \
                                                                                        \
        GraphicsCommandData commandData = {GraphicsCommandType::SET_UNIFORM, &command}; \
        Renderer::GetWindow()->ExecuteCommand(commandData);                             \
    }

    DEFINE_SET_UNIFORM(f32, UniformType::FLOAT);
    DEFINE_SET_UNIFORM(i32, UniformType::INT);
    DEFINE_SET_UNIFORM(u32, UniformType::UINT);
    DEFINE_SET_UNIFORM(Vec2, UniformType::VEC2);
    DEFINE_SET_UNIFORM(Vec3, UniformType::VEC3);
    DEFINE_SET_UNIFORM(Vec4, UniformType::VEC4);
    DEFINE_SET_UNIFORM(Mat2x2, UniformType::MAT2x2);
    DEFINE_SET_UNIFORM(Mat3x3, UniformType::MAT3x3);
    DEFINE_SET_UNIFORM(Mat4x4, UniformType::MAT4x4);
} // namespace rpp
