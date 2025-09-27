#include "modules/renderer/renderer_impl.h"
#include "core/array.h"

namespace rpp
{
    // TODO: Using command queue later.

    u32 Renderer::s_currentRendererIndex = INVALID_RENDERER_INDEX;
    Array<Renderer *> Renderer::s_currentRenderers = Array<Renderer *>();

    Renderer::Renderer()
        : Renderer(800, 600, "RPP Window")
    {
    }

    Renderer::Renderer(u32 width, u32 height, const char *title)
    {
        m_window = Graphics::CreateWindow(width, height, title);
        m_rendererId = s_currentRenderers.Size();
        s_currentRenderers.Push(this);
        s_currentRendererIndex = m_rendererId;
    }

    Renderer::~Renderer()
    {
        s_currentRenderers[m_rendererId] = nullptr;
    }

    void Renderer::Initialize()
    {
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
    }

    void Renderer::Active()
    {
        if (s_currentRendererIndex != m_rendererId)
        {
            s_currentRendererIndex = m_rendererId;
        }
    }

    void Renderer::PreDraw()
    {
        // Main loop
        m_window->PollEvents();

        // Clear color command
        ClearColorCommandData clearColorData = {{0.1f, 0.2f, 0.3f, 1.0f}};
        GraphicsCommandData commandData = {GraphicsCommandType::CLEAR_COLOR, &clearColorData};
        m_window->ExecuteCommand(commandData);
    }

    void Renderer::PostDraw()
    {
    }

    void Renderer::Present()
    {
        GraphicsCommandData swapBuffersCommand = {GraphicsCommandType::SWAP_BUFFERS, nullptr};
        m_window->ExecuteCommand(swapBuffersCommand);
    }

    Renderer &Renderer::GetCurrentRenderer()
    {
        if (s_currentRendererIndex == INVALID_RENDERER_INDEX || s_currentRendererIndex >= s_currentRenderers.Size())
        {
            throw std::runtime_error("No active renderer available");
        }
        return *s_currentRenderers[s_currentRendererIndex];
    }
} // namespace rpp
