#include "applications/graphic_session/manager.h"
#include "core/singleton_manager.h"
#include "applications/graphic_session/session.h"

namespace rpp
{
    RPP_SINGLETON_IMPLEMENT(GraphicSessionManager);

    GraphicSessionManager::GraphicSessionManager()
    {
        RPP_ASSERT(m_sessions == nullptr);

        m_sessions = CreateScope<Array<Scope<GraphicSession>>>();
    }

    GraphicSessionManager::~GraphicSessionManager()
    {
        RPP_ASSERT(m_sessions != nullptr);

        m_sessions.reset();
    }

    void GraphicSessionManager::AddSession(Scope<GraphicSession> session)
    {
        RPP_ASSERT(m_sessions != nullptr);
        RPP_ASSERT(session != nullptr);

        session->Initialize();
        m_sessions->Push(std::move(session));
    }

    b8 GraphicSessionManager::Update(f32 deltaTime)
    {
        RPP_ASSERT(m_sessions != nullptr);

        b8 shouldApplicationClose = TRUE;
        u32 numberOfSessions = m_sessions->Size();

        Queue<u32> closedSessionIndices;

        for (u32 sessionIndex = 0; sessionIndex < numberOfSessions; ++sessionIndex)
        {
            Scope<GraphicSession> &session = (*m_sessions)[sessionIndex];
            RPP_ASSERT(session != nullptr);

            u32 rendererId = session->GetRendererId();
            RPP_ASSERT(rendererId != INVALID_ID);
            Renderer::Activate(rendererId);

            if (Renderer::GetWindow()->ShouldWindowClose())
            {
                closedSessionIndices.Push(sessionIndex);
            }
            else
            {
                shouldApplicationClose = FALSE;
                session->Update(deltaTime);
                session->Render();
            }
        }

        while (!closedSessionIndices.Empty())
        {
            u32 indexToRemove = closedSessionIndices.Front();
            closedSessionIndices.Pop();

            Scope<GraphicSession> &session = (*m_sessions)[indexToRemove];
            session->Shutdown();

            m_sessions->Erase(indexToRemove);
        }

        return shouldApplicationClose;
    }

    void GraphicSessionManager::ClearSessions()
    {
        RPP_ASSERT(m_sessions != nullptr);

        u32 numberOfSessions = m_sessions->Size();
        for (u32 sessionIndex = 0; sessionIndex < numberOfSessions; ++sessionIndex)
        {
            Scope<GraphicSession> &session = (*m_sessions)[sessionIndex];
            RPP_ASSERT(session != nullptr);

            u32 rendererId = session->GetRendererId();
            RPP_ASSERT(rendererId != INVALID_ID);
            Renderer::Activate(rendererId);

            session->Shutdown();
        }

        m_sessions->Clear();
    }
} // namespace rpp
