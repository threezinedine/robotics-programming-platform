#pragma once
#include "modules/modules.h"

namespace rpp
{
    class GraphicSession;

    class RPP_SINGLETON GraphicSessionManager
    {
        RPP_SINGLETON_DEFINE(GraphicSessionManager);

    public:
        /**
         * After append the session into the manager, the manager will take the ownership of the session. And the window of the
         * session will be closed properly when the application ends.
         * @param session The graphic session to be added into the manager.
         */
        void AddSession(Scope<GraphicSession> session);

        /**
         * @brief Update all the graphic sessions inside the manager. This will call `Update` method of each session.
         * @param deltaTime The time elapsed since the last update call.
         *
         * @return TRUE if all the sessions are closed and the application should close, FALSE otherwise.
         */
        b8 Update(f32 deltaTime);

        /**
         * @brief Clear all the sessions inside the manager. This will call `Shutdown` method of each session.
         */
        void ClearSessions();

        /**
         * @brief Get the number of sessions inside the manager.
         * @return The number of sessions inside the manager.
         */
        u32 GetSessionCount() const;

        /**
         * @brief Get the session at the given index.
         * @param index The index of the session to get.
         * @return The session at the given index.
         * @note If the index is out of bounds, an exception will be thrown.
         */
        Scope<GraphicSession> &GetSession(u32 index) const;

    private:
        Scope<Array<Scope<GraphicSession>>> m_sessions; ///< All the graphic sessions inside the application.
    };
} // namespace rpp

#define CREATE_SESSION(SessionType, ...) \
    GraphicSessionManager::GetInstance()->AddSession(CreateScope<SessionType>(__VA_ARGS__))