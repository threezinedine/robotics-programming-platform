#include "modules/renderer/renderer_impl.h"
#include "core/containers/containers.h"
#include "modules/renderer/program.h"
#include "modules/renderer/rectangle.h"
#include "modules/renderer/imgui_internal.h"
#include "modules/renderer/line.h"
#include "modules/renderer/texture.h"

namespace rpp
{
    // TODO: Using command queue later.

    u32 Renderer::s_currentRendererIndex = INVALID_RENDERER_INDEX;
    Scope<Storage<Renderer::RendererData>> Renderer::s_currentRenderers = nullptr;

    void Renderer::Initialize()
    {
        RPP_LOG_DEBUG("Initializing rendering system.");

        RPP_ASSERT(s_currentRenderers == nullptr);
        if (!Graphics::Init())
        {
            throw std::runtime_error("Failed to initialize graphics backend");
        }

        s_currentRenderers = CreateScope<Storage<Renderer::RendererData>>();
        Program::Initialize();
        Texture::Initialize();
        Rectangle::Initialize();
        Line::Initialize();
        ImGuiImpl::Initialize();
        s_currentRendererIndex = INVALID_RENDERER_INDEX;
        RPP_LOG_DEBUG("Rendering system initialized successfully");
    }

    void Renderer::Shutdown()
    {
        RPP_LOG_DEBUG("Shutting down rendering system.");

        RPP_ASSERT(s_currentRenderers != nullptr);

        ImGuiImpl::Shutdown();
        Line::Shutdown();
        Rectangle::Shutdown();
        Program::Shutdown();
        Texture::Shutdown();
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
        RPP_ASSERT(s_currentRendererIndex != INVALID_RENDERER_INDEX);
        return s_currentRenderers->Get(s_currentRendererIndex);
    }

    u32 Renderer::CreateRenderer(u32 width, u32 height, const String &title)
    {
        RPP_ASSERT(s_currentRenderers != nullptr);
        u32 rendererId = s_currentRenderers->Create();
        RendererData *currentRenderer = s_currentRenderers->Get(rendererId);
        currentRenderer->window = Graphics::CreateWindow(width, height, title.CStr());

        ActivateRenderer(rendererId);
        currentRenderer->rectangleId = Rectangle::Create();
        currentRenderer->lineId = Line::Create();

        return rendererId;
    }

    void Renderer::DrawRectangle(const Rect &rect)
    {
        Rectangle::Draw(GetCurrentRenderer()->rectangleId, rect);
    }

    void Renderer::DrawLine(const Point &start, const Point &end)
    {
        Line::Draw(GetCurrentRenderer()->lineId, start, end);
    }

    void Renderer::ActivateRenderer(u32 renderId)
    {
        RPP_ASSERT(s_currentRenderers != nullptr);
        RPP_ASSERT(s_currentRenderers->Get(renderId) != nullptr)

        s_currentRendererIndex = renderId;

        // Activate context command
        ActivateContextCommandData activateContextData = {};
        GraphicsCommandData commandData = {GraphicsCommandType::ACTIVATE_CONTEXT, &activateContextData};
        GetCurrentRenderer()->window->ExecuteCommand(commandData);
    }

    void Renderer::DestroyRenderer(u32 renderId)
    {
        RPP_ASSERT(s_currentRenderers != nullptr);
        RPP_ASSERT(renderId != INVALID_RENDERER_INDEX);

        RendererData *data = s_currentRenderers->Get(renderId);

        RPP_ASSERT(data != nullptr);

        Rectangle::Destroy(data->rectangleId);
        Line::Destroy(data->lineId);

        s_currentRenderers->Free(renderId);
        if (s_currentRendererIndex == renderId)
        {
            s_currentRendererIndex = INVALID_RENDERER_INDEX;
        }
    }
} // namespace rpp
