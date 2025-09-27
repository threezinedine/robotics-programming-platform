#pragma once
#include "core/core.h"

namespace rpp
{
    /**
     * @brief Each graphics pipeline is represented by a Program object, which encapsulates the vertex and fragment shaders (maybe include compute shaders).
     * The Program class is responsible for compiling, linking, and managing the lifecycle of the shader program.
     */
    class Program
    {
    public:
        /**
         * @brief Constructs a Program object with the specified vertex and fragment shader source code.
         */
        Program();

        /**
         * @brief Constructs a Program object with the specified vertex and fragment shader source code.
         */
        Program(const String &vertexShaderSource, const String &fragmentShaderSource);

        /**
         * @brief Can be constructed only with a renderer instance, the actual program is created when the shader sources are provided.
         */
        Program(const Program &) = delete;

        /**
         * @brief Destructs the Program and frees associated resources.
         */
        ~Program();

    public:
        /**
         * @brief Use for activating the graphics pipeline (shader program) for rendering.
         */
        void Use() const;

    private:
        u32 m_programId; ///< The unique identifier for the program in the graphics API.
    };
} // namespace rpp
