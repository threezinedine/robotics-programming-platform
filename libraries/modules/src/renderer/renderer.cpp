#include "modules/renderer/renderer_impl.h"
#include "core/containers/containers.h"
#include "modules/renderer/program.h"
#include "modules/renderer/rectangle.h"
#include "modules/renderer/imgui_internal.h"

namespace rpp
{
    // TODO: Using command queue later.

    u32 Renderer::s_currentRendererIndex = INVALID_RENDERER_INDEX;
    Scope<Storage<Renderer::RendererData>> Renderer::s_currentRenderers = nullptr;

    void Renderer::Initialize()
    {
        RPP_ASSERT(s_currentRenderers == nullptr);
        if (!Graphics::Init())
        {
            throw std::runtime_error("Failed to initialize graphics backend");
        }
        s_currentRenderers = CreateScope<Storage<Renderer::RendererData>>();
        Program::Initialize();
        Rectangle::Initialize();
        ImGuiImpl::Initialize();
        s_currentRendererIndex = INVALID_RENDERER_INDEX;
        RPP_LOG_DEBUG("Rendering system initialized successfully");
    }

    void Renderer::Shutdown()
    {
        RPP_ASSERT(s_currentRenderers != nullptr);

        ImGuiImpl::Shutdown();
        Rectangle::Shutdown();
        Program::Shutdown();
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
        RPP_ASSERT(s_currentRenderers != nullptr);

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

        // Activate context command
        ActivateContextCommandData activateContextData = {};
        GraphicsCommandData commandData = {GraphicsCommandType::ACTIVATE_CONTEXT, &activateContextData};
        GetCurrentRenderer()->window->ExecuteCommand(commandData);
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
