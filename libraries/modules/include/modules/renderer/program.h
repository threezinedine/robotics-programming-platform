#pragma once
#include "core/core.h"

namespace rpp
{
    /**
     * @brief Each graphics pipeline is represented by a Program object, which encapsulates the vertex and fragment shaders (maybe include compute shaders).
     * The Program class is responsible for compiling, linking, and managing the lifecycle of the shader program.
     */
    class RPP_PYTHON_BINDING Program
    {
    private:
        /**
         * @brief All information related to a graphics pipeline (shader program).
         */
        struct ProgramData
        {
            u32 rendererId; ///< The ID of the renderer that created this program.
            u32 programId;  ///< The unique identifier for the program in the graphics API.
        };

    public:
        /**
         * @brief Initialize the Program storage, should be called once before using any Program related features.
         */
        static void Initialize();

        /**
         * @brief Shutdown the Program storage, must be called once before exiting the application.
         */
        static void Shutdown();

    public:
        /**
         * @brief Create a new graphics pipeline (shader program) from vertex and fragment shader source code.
         *
         * @param vertexSource The source code for the vertex shader.
         * @param fragmentSource The source code for the fragment shader.
         */
        static u32 Create(const String &vertexSource, const String &fragmentSource) RPP_PYTHON_BINDING;

        /**
         * @brief Destroy the graphics pipeline (shader program) and free associated resources.
         */
        static void Destroy(u32 programId) RPP_PYTHON_BINDING;

        /**
         * @brief Use for activating the graphics pipeline (shader program) for rendering.
         */
        static void Use(u32 programId) RPP_PYTHON_BINDING;

        /**
         * @brief Set a float uniform variable in the shader program. The current program must be active (used) before calling this method.
         *
         * @param name The name of the uniform variable in the shader.
         * @param value The value to set for the uniform variable.
         */
        template <typename T>
        static void SetUniform(const String &name, T value);

    private:
        static Scope<Storage<ProgramData>> s_programs; ///< Storage for all created program IDs.
        static u32 s_currentProgramId;                 ///< The currently active program ID.
    };
} // namespace rpp
