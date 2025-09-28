#include "modules/renderer/renderer_impl.h"
#include "core/containers/containers.h"

namespace rpp
{
    // TODO: Using command queue later.

    u32 Renderer::s_currentRendererIndex = INVALID_RENDERER_INDEX;
    Array<Renderer *> Renderer::s_currentRenderers = Array<Renderer *>();

    Renderer::Renderer()
        : Renderer(800, 600, "RPP Window")
    {
    }

    Renderer::Renderer(u32 width, u32 height, const String &title)
    {
        RPP_LOG_DEBUG("Creating renderer with window size {}x{} and title '{}'", width, height, title);
        m_window = Graphics::CreateWindow(width, height, title.CStr());
        m_rendererId = s_currentRenderers.Size();
        s_currentRenderers.Push(this);
        s_currentRendererIndex = m_rendererId;
    }

    Renderer::~Renderer()
    {
        RPP_LOG_DEBUG("Destroying renderer with ID {}", m_rendererId);
        s_currentRenderers[m_rendererId] = nullptr;
    }

    void Renderer::Initialize()
    {
        RPP_LOG_INFO("Initializing the rendering system");
        if (!Graphics::Init())
        {
            throw std::runtime_error("Failed to initialize graphics backend");
        }
    }

    void Renderer::Shutdown()
    {
        s_currentRenderers.Clear();
        s_currentRenderers.~Array();
        Graphics::Shutdown();
        RPP_LOG_INFO("Rendering system shutdown complete");
    }

    void Renderer::PreDrawImpl()
    {
        // Main loop
        m_window->PollEvents();

        // Clear color command
        ClearColorCommandData clearColorData = {{0.1f, 0.2f, 0.3f, 1.0f}};
        GraphicsCommandData commandData = {GraphicsCommandType::CLEAR_COLOR, &clearColorData};
        m_window->ExecuteCommand(commandData);
    }

    void Renderer::PreDraw()
    {
        GetCurrentRenderer()->PreDrawImpl();
    }

    void Renderer::PostDraw()
    {
        GetCurrentRenderer()->PostDrawImpl();
    }

    void Renderer::PostDrawImpl()
    {
        // Placeholder for post-draw operations
    }

    void Renderer::PresentImpl()
    {
        GraphicsCommandData swapBuffersCommand = {GraphicsCommandType::SWAP_BUFFERS, nullptr};
        m_window->ExecuteCommand(swapBuffersCommand);
    }

    void Renderer::Present()
    {
        GetCurrentRenderer()->PresentImpl();
    }

    Renderer *Renderer::GetCurrentRenderer()
    {
        if (s_currentRendererIndex == INVALID_RENDERER_INDEX || s_currentRendererIndex >= s_currentRenderers.Size())
        {
            return nullptr;
        }
        return s_currentRenderers[s_currentRendererIndex];
    }

    u32 Renderer::CreateRenderer(u32 width, u32 height, const String &title)
    {
        RPP_NEW(Renderer(width, height, title));
        return s_currentRendererIndex;
    }

    void Renderer::ActivateRenderer(u32 renderId)
    {
        if (renderId >= s_currentRenderers.Size() || s_currentRenderers[renderId] == nullptr)
        {
            return;
        }

        s_currentRendererIndex = renderId;
    }

    void Renderer::DestroyRenderer(u32 renderId)
    {
        if (renderId >= s_currentRenderers.Size() || s_currentRenderers[renderId] == nullptr)
        {
            throw std::runtime_error("Invalid renderer ID");
        }

        delete s_currentRenderers[renderId];
        s_currentRenderers[renderId] = nullptr;
        if (s_currentRendererIndex == renderId)
        {
            s_currentRendererIndex = INVALID_RENDERER_INDEX;
        }
    }
} // namespace rpp
