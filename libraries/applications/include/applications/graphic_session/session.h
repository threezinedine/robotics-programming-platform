#pragma once
#include "modules/modules.h"

namespace rpp
{
    /**
     * @brief Each new window inside the application must be inherited from this class, via `GraphicSessionManager`, all the
     * windows will be managed by the manager and be closed properly when the application ends.
     */
    class GraphicSession
    {
    public:
        /**
         * @brief Construct a new Graphic Session object. Each graphic session contains a renderer instance.
         * @param width The width of the window.
         * @param height The height of the window.
         * @param title The title of the window.
         * @param enableImGui Whether to enable ImGui for this window. Default is FALSE.
         * @param disableMouse Whether to disable mouse input for this window. Default is FALSE.
         */
        GraphicSession(u32 width, u32 height, const String &title, b8 enableImGui = FALSE);
        virtual ~GraphicSession();

    public:
        /**
         * @brief Initialize the graphic session. This will create the window and the renderer instance.
         * It will call `InitializeImpl` to allow derived classes to implement their own initialization logic
         * after the window and renderer are created. Must be called before any other methods.
         */
        void Initialize();

        /**
         * @brief Update the graphic session. This will poll events and update the window.
         * It will call `UpdateImpl` to allow derived classes to implement their own update logic.
         * @param deltaTime The time elapsed since the last update call.
         * @note This method should be called in the main loop of the application.
         */
        void Update(f32 deltaTime);

        /**
         * @brief Render the graphic session. This will clear the window and present the rendered content.
         * It will call `RenderImpl` to allow derived classes to implement their own rendering logic.
         * @note This method should be called after `Update` in the main loop of the application.
         */
        void Render();

        /**
         * @brief Shutdown the graphic session. This will destroy the window and the renderer instance.
         * It will call `ShutdownImpl` to allow derived classes to implement their own shutdown logic
         * before the window and renderer are destroyed. Must be called when the graphic session is no longer needed.
         */
        void Shutdown();

    protected:
        /**
         * @brief Implementation of the initialization logic for derived classes.
         * This method is called by `Initialize` after the window and renderer are created.
         * Derived classes should override this method to implement their own initialization logic.
         */
        virtual void InitializeImpl();

        /**
         * @brief Implementation of the update logic for derived classes.
         * This method is called by `Update` to allow derived classes to implement their own update logic.
         * @param deltaTime The time elapsed since the last update call.
         */
        virtual void UpdateImpl(f32 deltaTime);

        /**
         * @brief Implementation of the rendering logic for derived classes.
         * This method is called by `Render` to allow derived classes to implement their own rendering logic.
         */
        virtual void RenderImpl();

        /**
         * @brief Implementation of the shutdown logic for derived classes.
         * This method is called by `Shutdown` before the window and renderer are destroyed.
         * Derived classes should override this method to implement their own shutdown logic.
         */
        virtual void ShutdownImpl();

    public:
        /**
         * @brief Get the renderer id associated with this graphic session.
         * @return The renderer id.
         */
        inline u32 GetRendererId() const { return m_rendererId; }

    protected:
        u32 m_rendererId; ///< Each graphic session contains a renderer instance.

    private:
        u32 m_width;      ///< The width of the window.
        u32 m_height;     ///< The height of the window.
        String m_title;   ///< The title of the window.
        b8 m_enableImGui; ///< Whether to enable ImGui for this window.
    };
} // namespace rpp
