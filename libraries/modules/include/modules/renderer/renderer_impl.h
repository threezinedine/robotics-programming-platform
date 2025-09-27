#pragma once
#include "core/core.h"

namespace rpp
{
    class Rectangle;

    /**
     * @brief Instance for working with drawing in 1 window (each window has its own renderer as 1-1 relation).
     */
    class Renderer
    {
    public:
        Renderer(u32 width, u32 height, const char *title);
        Renderer(const Renderer &);
        ~Renderer();

    public:
        /**
         * @brief Create a  renderer instance (a window is also created).
         */
        static Renderer Create(u32 width, u32 height, const char *title);

    public:
        void PreDraw();
        void PostDraw();

        void Present();

    public:
        inline Scope<Window> &GetWindow() { return m_window; }

    private:
        Scope<Window> m_window; ///< The window associated with this renderer.
    };
} // namespace rpp
