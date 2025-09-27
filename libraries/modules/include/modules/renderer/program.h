#pragma once
#include "core/core.h"

namespace rpp
{
    class Renderer;

    class Program
    {
    public:
        Program(Renderer &renderer);
        Program(Renderer &renderer, const String &vertexShaderSource, const String &fragmentShaderSource);

        Program(const Program &) = delete;
        ~Program();

    public:
        void Use() const;

    private:
        u32 m_programId;      ///< The unique identifier for the program in the graphics API.
        Renderer &m_renderer; ///< Reference to the associated renderer instance.
    };
} // namespace rpp
