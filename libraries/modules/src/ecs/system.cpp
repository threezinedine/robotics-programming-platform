#include "modules/ecs/system.h"

namespace rpp
{
    void System::Initial(ECSId ecsId)
    {
        InitialImpl(ecsId);
    }

    void System::Update(ECSId ecsId, f32 deltaTime)
    {
        UpdateImpl(ecsId, deltaTime);
    }

    void System::Shutdown(ECSId ecsId)
    {
        ShutdownImpl(ecsId);
    }

    void System::InitialImpl(ECSId ecsId)
    {
        RPP_UNUSED(ecsId);
    }

    void System::UpdateImpl(ECSId ecsId, f32 deltaTime)
    {
        RPP_UNUSED(ecsId);
        RPP_UNUSED(deltaTime);
    }

    void System::ShutdownImpl(ECSId ecsId)
    {
        RPP_UNUSED(ecsId);
    }
} // namespace rpp
