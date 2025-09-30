#pragma once
#include "core/core.h"
#include "modules/storage.h"
#include "modules/renderer/type.h"

#define INVALID_RENDERER_INDEX u32(-1)

namespace rpp
{
    /**
     * @brief Instance for working with drawing in 1 window (each window has its own renderer as 1-1 relation).
     */
    class RPP_PYTHON_BINDING Renderer
    {
    private:
        /**
         * All information related to the renderer instance.
         */
        struct RendererData
        {
            Scope<Window> window; ///< The window associated with this renderer.
            u32 rendererId;       ///< The unique identifier for the renderer instance.
            u32 rectangleId;      ///< The id of the rectangle used for drawing.
            u32 lineId;           ///< The id of the line used for drawing.
        };

    public:
        /**
         * @brief Initialize the rendering system, should be called once before using any rendering related features.
         */
        static void Initialize() RPP_PYTHON_BINDING;

        /**
         * @brief Shutdown the rendering system, must be called once before exiting the application.
         */
        static void Shutdown() RPP_PYTHON_BINDING;

    private:
        /**
         * Each time a renderer is created, it is added to this array. When Active() is called, it deactivates all other renderers.
         */
        static Scope<Storage<RendererData>> s_currentRenderers;

        static u32 s_currentRendererIndex; ///< The index of the currently active renderer. If no renderer is active, it is set to `INVALID_RENDERER_INDEX`.

        /**
         * Used for other components to get the current active renderer. If no renderer is active, an exception will be thrown.
         */
        static RendererData *GetCurrentRenderer();

    public:
        /**
         * @brief Get the window associated with this renderer.
         *
         * @return The window instance.
         */
        inline static Scope<Window> &GetWindow() { return GetCurrentRenderer()->window; }

        /**
         * @brief Get the id of the current active renderer.
         * @return The id of the current active renderer. If a program or rectangle needs to execute a graphics command, it can use this id to get the current active renderer.
         */
        inline static u32 GetCurrentRendererId() { return s_currentRendererIndex; }

        /**
         * @brief Create a new renderer instance with its own window.
         *
         * @param width The width of the window.
         * @param height The height of the window.
         * @param title The title of the window.
         *
         * @return The id of the created renderer instance. This id is used to activate or destroy the renderer later.
         *
         * @note The created renderer is not active by default. You need to call `ActivateRenderer` with the returned id to make it active.
         */
        static u32 CreateRenderer(u32 width, u32 height, const String &title) RPP_PYTHON_BINDING;

        /**
         * @brief Activate the renderer with the given id. This will deactivate any other active renderer.
         */
        static void ActivateRenderer(u32 renderId) RPP_PYTHON_BINDING;

        /**
         * @brief Draw a rectangle on the current active renderer.
         */
        static void DrawRectangle(const Rect &rect);

        /**
         * @brief Draw a line on the current active renderer.
         */
        static void DrawLine(const Point &start, const Point &end);

        /**
         * @brief Destroy the renderer with the given id. If the renderer is currently active, it will be deactivated first.
         *
         * @param renderId The id of the renderer to destroy.
         *
         * @throws std::runtime_error if the renderId is invalid.
         */
        static void DestroyRenderer(u32 renderId) RPP_PYTHON_BINDING;

        /**
         * @brief All operations to be done before drawing the frame.
         */
        static void PreDraw() RPP_PYTHON_BINDING;

        /**
         * @brief All operations to be done after drawing the frame.
         */
        static void PostDraw() RPP_PYTHON_BINDING;

        /**
         * @brief Present the rendered frame to the window.
         */
        static void Present() RPP_PYTHON_BINDING;
    };
} // namespace rpp
