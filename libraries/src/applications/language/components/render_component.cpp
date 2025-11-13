#include "applications/language/components/render_component.h"

namespace rpp
{
    RenderSystem::RenderSystem()
    {
        RPP_PROFILE_SCOPE();
    }

    RenderSystem::~RenderSystem()
    {
        RPP_PROFILE_SCOPE();
    }

    void RenderSystem::UpdateImpl(ECSId ecsId, EntityId entityId, f32 deltaTime)
    {
        RPP_PROFILE_SCOPE();
        RPP_UNUSED(ecsId);
        RPP_UNUSED(entityId);
        RPP_UNUSED(deltaTime);
    }
} // namespace rpp
