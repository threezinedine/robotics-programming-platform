#include "modules/ecs/system.h"

namespace rpp
{
    void System::Initial(ECSId ecsId, EntityId entityId)
    {
        RPP_PROFILE_SCOPE();
        InitialImpl(ecsId, entityId);
    }

    void System::Suspend(ECSId ecsId, EntityId entityId)
    {
        RPP_PROFILE_SCOPE();
        SuspendImpl(ecsId, entityId);
    }

    void System::Update(ECSId ecsId, EntityId entityId, f32 deltaTime)
    {
        RPP_PROFILE_SCOPE();
        UpdateImpl(ecsId, entityId, deltaTime);
    }

    void System::Resume(ECSId ecsId, EntityId entityId)
    {
        RPP_PROFILE_SCOPE();
        ResumeImpl(ecsId, entityId);
    }

    void System::Shutdown(ECSId ecsId, EntityId entityId)
    {
        RPP_PROFILE_SCOPE();
        ShutdownImpl(ecsId, entityId);
    }

    void System::InitialImpl(ECSId ecsId, EntityId entityId)
    {
        RPP_PROFILE_SCOPE();
        RPP_UNUSED(ecsId);
        RPP_UNUSED(entityId);
    }

    void System::SuspendImpl(ECSId ecsId, EntityId entityId)
    {
        RPP_PROFILE_SCOPE();
        RPP_UNUSED(ecsId);
        RPP_UNUSED(entityId);
    }

    void System::UpdateImpl(ECSId ecsId, EntityId entityId, f32 deltaTime)
    {
        RPP_PROFILE_SCOPE();
        RPP_UNUSED(ecsId);
        RPP_UNUSED(entityId);
        RPP_UNUSED(deltaTime);
    }

    void System::ResumeImpl(ECSId ecsId, EntityId entityId)
    {
        RPP_PROFILE_SCOPE();
        RPP_UNUSED(ecsId);
        RPP_UNUSED(entityId);
    }

    void System::ShutdownImpl(ECSId ecsId, EntityId entityId)
    {
        RPP_PROFILE_SCOPE();
        RPP_UNUSED(ecsId);
        RPP_UNUSED(entityId);
    }
} // namespace rpp
