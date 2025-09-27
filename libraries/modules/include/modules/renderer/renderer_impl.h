#pragma once
#include "core/core.h"

#define INVALID_RENDERER_INDEX u32(-1)

namespace rpp
{
    /**
     * @brief Instance for working with drawing in 1 window (each window has its own renderer as 1-1 relation).
     */
    class RPP_PYTHON_BINDING Renderer
    {
    public:
        /**
         * @brief Constructs a default renderer. The default width, height, and title of the window are 800, 600, and 'RPP Window' respectively.
         */
        Renderer();

        /**
         * @brief Constructs a renderer with a window of the specified width, height, and title.
         * @param width The width of the window.
         * @param height The height of the window.
         * @param title The title of the window.
         */
        Renderer(u32 width, u32 height, const String &title);

        Renderer(const Renderer &) = delete;

        /**
         * @brief Destructs the renderer and frees associated resources.
         */
        ~Renderer();

    public:
        /**
         * @brief Initialize the rendering system, should be called once before using any rendering related features.
         */
        static void Initialize() RPP_PYTHON_BINDING;

        /**
         * @brief Shutdown the rendering system, must be called once before exiting the application.
         */
        static void Shutdown() RPP_PYTHON_BINDING;

    public:
        /**
         * @brief Use each time before working with a specific window (if multiple windows are used). After this method is called,
         * all rendering commands will be directed to this window until another window's Active() method is called.
         */
        void Active() RPP_PYTHON_BINDING;

    public:
        /**
         * @brief Doing some operations which must be run before drawing. E.g., polling events, clearing the screen, etc.
         */
        void PreDraw() RPP_PYTHON_BINDING;

        /**
         * @brief Doing some operations which must be run after drawing. E.g., post-processing, etc.
         */
        void PostDraw() RPP_PYTHON_BINDING;

        /**
         * @brief Present the rendered image to the screen (swap buffers).
         */
        void Present() RPP_PYTHON_BINDING;

    public:
        /**
         * @brief Get the window associated with this renderer.
         *
         * @return The window instance.
         */
        inline Scope<Window> &GetWindow() { return m_window; }

    private:
        /**
         * Each time a renderer is created, it is added to this array. When Active() is called, it deactivates all other renderers.
         */
        static Array<Renderer *> s_currentRenderers;

        static u32 s_currentRendererIndex; ///< The index of the currently active renderer. If no renderer is active, it is set to `INVALID_RENDERER_INDEX`.

    public:
        /**
         * Used for other components to get the current active renderer. If no renderer is active, an exception will be thrown.
         */
        static Renderer &GetCurrentRenderer();

    private:
        Scope<Window> m_window; ///< The window associated with this renderer.
        u32 m_rendererId;       ///< The unique identifier for the renderer instance.
    };
} // namespace rpp
