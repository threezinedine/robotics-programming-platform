#include "modules/renderer/renderer_impl.h"
#include "core/containers/containers.h"

namespace rpp
{
    // TODO: Using command queue later.

    u32 Renderer::s_currentRendererIndex = INVALID_RENDERER_INDEX;
    Scope<Storage<Renderer::RendererData>> Renderer::s_currentRenderers = nullptr;

    void Renderer::Initialize()
    {
        RPP_LOG_INFO("Initializing the rendering system");
        if (!Graphics::Init())
        {
            throw std::runtime_error("Failed to initialize graphics backend");
        }
        s_currentRenderers = CreateScope<Storage<Renderer::RendererData>>();
        RPP_LOG_DEBUG("Rendering system initialized successfully");
    }

    void Renderer::Shutdown()
    {
        s_currentRenderers.reset();
        Graphics::Shutdown();
        RPP_LOG_INFO("Rendering system shutdown complete");
    }

    void Renderer::PreDraw()
    {
        RendererData *current = GetCurrentRenderer();
        // Main loop
        current->window->PollEvents();

        // Clear color command
        ClearColorCommandData clearColorData = {{0.1f, 0.2f, 0.3f, 1.0f}};
        GraphicsCommandData commandData = {GraphicsCommandType::CLEAR_COLOR, &clearColorData};
        current->window->ExecuteCommand(commandData);
    }

    void Renderer::PostDraw()
    {
        // Currently nothing to do here.
    }

    void Renderer::Present()
    {
        RendererData *current = GetCurrentRenderer();
        GraphicsCommandData swapBuffersCommand = {GraphicsCommandType::SWAP_BUFFERS, nullptr};
        current->window->ExecuteCommand(swapBuffersCommand);
    }

    Renderer::RendererData *Renderer::GetCurrentRenderer()
    {
        if (s_currentRendererIndex == INVALID_RENDERER_INDEX)
        {
            return nullptr;
        }
        return s_currentRenderers->Get(s_currentRendererIndex);
    }

    u32 Renderer::CreateRenderer(u32 width, u32 height, const String &title)
    {
        RPP_ASSERT(s_currentRenderers != nullptr);
        u32 rendererId = s_currentRenderers->Create();
        RendererData *currentRenderer = s_currentRenderers->Get(rendererId);
        currentRenderer->window = Graphics::CreateWindow(width, height, title.CStr());
        s_currentRendererIndex = rendererId;
        return s_currentRendererIndex;
    }

    void Renderer::ActivateRenderer(u32 renderId)
    {
        RPP_ASSERT(s_currentRenderers != nullptr);
        if (s_currentRenderers->Get(renderId) == nullptr)
        {
            return;
        }

        s_currentRendererIndex = renderId;
    }

    void Renderer::DestroyRenderer(u32 renderId)
    {
        RPP_ASSERT(s_currentRenderers != nullptr);
        if (s_currentRenderers->Get(renderId) == nullptr)
        {
            throw std::runtime_error("Invalid renderer ID");
        }

        s_currentRenderers->Free(renderId);
        if (s_currentRendererIndex == renderId)
        {
            s_currentRendererIndex = INVALID_RENDERER_INDEX;
        }
    }
} // namespace rpp
