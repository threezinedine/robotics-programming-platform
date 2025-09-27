#include "modules/renderer/renderer_impl.h"

namespace rpp
{
    Renderer::Renderer(u32 width, u32 height, const char *title)
    {
        m_window = Graphics::CreateWindow(width, height, title);
    }

    Renderer::Renderer(const Renderer &)
    {
    }

    Renderer::~Renderer()
    {
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
} // namespace rpp
