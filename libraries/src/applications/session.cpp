#include "applications/graphic_session/session.h"

namespace rpp
{
    GraphicSession::GraphicSession(u32 width, u32 height, const String &title)
        : m_rendererId(INVALID_ID), m_width(width),
          m_height(height), m_title(title)
    {
    }

    void GraphicSession::Initialize()
    {
        m_rendererId = Renderer::Create(m_width, m_height, m_title);
        RPP_ASSERT(m_rendererId != INVALID_ID);
        Renderer::Activate(m_rendererId);
        InitializeImpl();
    }

    void GraphicSession::Update(f32 deltaTime)
    {
        RPP_ASSERT(m_rendererId != INVALID_ID);
        Renderer::Activate(m_rendererId);
        InputSystem::Update(deltaTime);
        UpdateImpl(deltaTime);
    }

    void GraphicSession::Render()
    {
        RPP_ASSERT(m_rendererId != INVALID_ID);
        Renderer::Activate(m_rendererId);

        ;
        Renderer::PreDraw();

        ;
        RenderImpl();

        ;
        Renderer::PostDraw();

        ;
        Renderer::Present();
    }

    void GraphicSession::Shutdown()
    {
        RPP_ASSERT(m_rendererId != INVALID_ID);
        Renderer::Activate(m_rendererId);
        ShutdownImpl();
        Renderer::Destroy(m_rendererId);
        m_rendererId = INVALID_ID;
    }

    void GraphicSession::InitializeImpl()
    {
        // Default implementation does nothing.
    }

    void GraphicSession::UpdateImpl(f32 deltaTime)
    {
        // Default implementation does nothing.
    }

    void GraphicSession::RenderImpl()
    {
        // Default implementation does nothing.
    }

    void GraphicSession::ShutdownImpl()
    {
        // Default implementation does nothing.
    }

    GraphicSession::~GraphicSession()
    {
    }
} // namespace rpp
