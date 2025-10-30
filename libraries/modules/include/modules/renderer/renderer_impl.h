#pragma once
#include "core/core.h"
#include "modules/renderer/type.h"

#define INVALID_RENDERER_INDEX u32(-1)

namespace rpp
{
    class ImGuiImpl;

    /**
     * The stored information related to the graphics API used by the renderer. This data will be stored in the Graphics's Window instance.
     */
    struct RendererGraphicData
    {
        u32 rendererId; ///< The unique identifier for the renderer instance.
        u32 circleMask; ///< The circle mask texture id used for drawing circles.
    };

    /**
     * @brief Instance for working with drawing in 1 window (each window has its own renderer as 1-1 relation).
     */
    class Renderer
    {
    public:
        friend class ImGuiImpl;

    private:
        /**
         * All information related to the renderer instance.
         */
        struct RendererData
        {
            Scope<Window> window; ///< The window associated with this renderer.
            u32 rendererId;       ///< The unique identifier for the renderer instance.
            u32 rectangleId;      ///< The id of the rectangle used for drawing.
            u32 imguiId;          ///< The id of the imgui instance used for drawing.
            u32 lineId;           ///< The id of the line used for drawing.
            u32 circleMask;       ///< The circle mask texture id used for drawing circles.
            u32 rectangleMask;    ///< The rectangle mask texture id used for drawing rectangles (the default mask).
            u32 mouseTexture;     ///< The texture id used for the mouse cursor.
        };

    public:
        /**
         * @brief Initialize the rendering system, should be called once before using any rendering related features.
         */
        static void Initialize();

        /**
         * @brief Shutdown the rendering system, must be called once before exiting the application.
         */
        static void Shutdown();

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
         * @param useImGui Whether to initialize ImGui for this renderer. Default is FALSE.
         * @param disableMouse Whether to disable mouse input for this window. Default is FALSE.
         *
         * @return The id of the created renderer instance. This id is used to activate or destroy the renderer later.
         *
         * @note The created renderer is not active by default. You need to call `Activate` with the returned id to make it active.
         */
        static u32 Create(u32 width, u32 height, const String &title, b8 useImGui = FALSE);

        /**
         * @brief Activate the renderer with the given id. This will deactivate any other active renderer.
         */
        static void Activate(u32 renderId);

        /**
         * @brief Draw a rectangle on the current active renderer.
         * @param rect The rectangle parameters including center coordinates, width, and height.
         * @param textureId The ID of the texture to apply to the rectangle. If INVALID_ID, no texture is applied.
         * @param maskTextureId The ID of the mask texture to apply to the rectangle. If INVALID_ID, no mask is applied.
         */
        static void DrawRectangle(const Rect &rect, u32 textureId = INVALID_ID, u32 maskTextureId = INVALID_ID);

        /**
         * @brief Draw a circle on the current active renderer.
         * @param center The center point of the circle.
         * @param radius The radius of the circle.
         * @param textureId The ID of the texture to apply to the circle. If INVALID_ID, no texture is applied.
         */
        static void DrawCircle(const Point &center, f32 radius, u32 textureId = INVALID_ID);

        /**
         * @brief Draw a line on the current active renderer.
         */
        static void DrawLine(const Point &start, const Point &end);

        /**
         * @brief Draw the mouse cursor at the specified position on the current active renderer.
         * @param position The position to draw the mouse cursor.
         * @note This function requires a mouse texture to be loaded in the renderer.
         */
        static void DrawMouseCursor(const Point &position);

        /**
         * @brief Draw the rendering scene inside the ImGui frame. This function can be used to embed
         * a rendering scene within an ImGui window.
         */
        static void DrawingSceneInImGui();

        /**
         * @brief Close the window associated with the current active renderer.
         * After calling this function, the renderer will no longer be usable.
         */
        static void CloseWindow();

        /**
         * @brief Destroy the renderer with the given id. If the renderer is currently active, it will be deactivated first.
         *
         * @param renderId The id of the renderer to destroy.
         *
         * @throws std::runtime_error if the renderId is invalid.
         */
        static void Destroy(u32 renderId);

        /**
         * @brief All operations to be done before drawing the frame.
         */
        static void PreDraw();

        /**
         * @brief All operations to be done after drawing the frame.
         */
        static void PostDraw();

        /**
         * @brief Present the rendered frame to the window.
         */
        static void Present();
    };
} // namespace rpp
