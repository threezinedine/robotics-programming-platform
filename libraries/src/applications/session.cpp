#include "applications/graphic_session/session.h"

namespace rpp
{
    GraphicSession::GraphicSession(u32 width, u32 height, const String &title)
        : m_rendererId(INVALID_ID), m_width(width),
          m_height(height), m_title(title)
    {
        RPP_PROFILE_SCOPE();
    }

    void GraphicSession::Initialize()
    {
        RPP_PROFILE_SCOPE();
        m_rendererId = Renderer::Create(m_width, m_height, m_title);
        RPP_ASSERT(m_rendererId != INVALID_ID);
        Renderer::Activate(m_rendererId);
        InitializeImpl();
    }

    void GraphicSession::Update(f32 deltaTime)
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(m_rendererId != INVALID_ID);
        Renderer::Activate(m_rendererId);
        InputSystem::Update(deltaTime);
        UpdateImpl(deltaTime);
    }

    void GraphicSession::Render()
    {
        RPP_PROFILE_SCOPE();
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
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(m_rendererId != INVALID_ID);
        Renderer::Activate(m_rendererId);
        ShutdownImpl();
        Renderer::Destroy(m_rendererId);
        m_rendererId = INVALID_ID;
    }

    void GraphicSession::InitializeImpl()
    {
        RPP_PROFILE_SCOPE();
        // Default implementation does nothing.
    }

    void GraphicSession::UpdateImpl(f32 deltaTime)
    {
        RPP_PROFILE_SCOPE();
        // Default implementation does nothing.
    }

    void GraphicSession::RenderImpl()
    {
        RPP_PROFILE_SCOPE();
        // Default implementation does nothing.
    }

    void GraphicSession::ShutdownImpl()
    {
        RPP_PROFILE_SCOPE();
        // Default implementation does nothing.
    }

    GraphicSession::~GraphicSession()
    {
        RPP_PROFILE_SCOPE();
    }
} // namespace rpp
