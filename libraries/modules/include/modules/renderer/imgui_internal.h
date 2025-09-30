#pragma once
#include "core/core.h"
#include "modules/storage.h"
#include "imgui.h"

namespace rpp
{
    class ImGuiImpl
    {
    private:
        struct ImGuiData
        {
            u32 rendererId;          ///< The id of the renderer associated with this ImGui instance.
            ImGuiIO *io;             ///< The ImGuiIO structure for input/output handling.
            u32 frameBufferId;       ///< The ID of the framebuffer in the graphics API.
            u32 textureId;           ///< The ID of the texture used by ImGui.
            u32 frameRenderBufferId; ///< The ID of the renderbuffer in the graphics API.
        };

    public:
        /**
         * @brief Initialize the ImGui storage, should be called once before using any ImGui related features.
         */
        static void Initialize();

        /**
         * @brief Shutdown the ImGui storage, must be called once before exiting the application.
         */
        static void Shutdown();

    public:
        /**
         * @brief Create a new ImGui instance.
         */
        static u32 Create();

        /**
         * @brief Prepare for a new ImGui frame. This should be called at the beginning of each frame before any ImGui calls.
         * It sets up the necessary state for ImGui to function correctly within the rendering context.
         *
         * @param imguiId The ID of the ImGui instance to prepare the frame for.
         */
        static void PrepareFrame(u32 imguiId);

        /**
         * @brief Render the ImGui draw data. This should be called after all ImGui calls for the frame have been made.
         * It processes the ImGui draw data and issues the necessary rendering commands to display the Im
         * Gui interface.
         * @param imguiId The ID of the ImGui instance to render.
         */
        static void Render(u32 imguiId);

        /**
         * @brief Draw the rendering scene inside the ImGui frame. This function can be used to embed
         * a rendering scene within an ImGui window.
         */
        static void DrawRenderingScene(u32 imguiId);

        /**
         * @brief Destroy the ImGui instance and free associated resources.
         *
         * @param imguiId The ID of the ImGui instance to destroy.
         */
        static void Destroy(u32 imguiId);

    private:
        static Scope<Storage<ImGuiData>> s_imguis; ///< Storage for all created ImGui instances.
    };
} // namespace rpp
