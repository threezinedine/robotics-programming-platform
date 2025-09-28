#pragma once
#include "core/core.h"
#include "modules/storage.h"

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
            u32 rendererId; ///< The id of the renderer that created this program.
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

    private:
        static Scope<Storage<ProgramData>> s_programs; ///< Storage for all created program IDs.
    };
} // namespace rpp
