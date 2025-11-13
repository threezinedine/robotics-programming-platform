#pragma once
#include "modules/modules.h"

namespace rpp
{
    /**
     * If the entity has to be drawn, append this component to it, with the necessary data.
     * This component is processed by the RenderSystem to draw the entity in the language editor.
     */
    struct RenderComponent
    {
        f32 x;         ///< Position X in the editor space, not NULL.
        f32 y;         ///< Position Y in the editor space, not NULL.
        u32 textureId; ///< Nullable, if `NULL`, a white rectangle will be drawn.
        u32 maskId;    ///< Nullable, if `NULL`, no mask will be applied.
    };

    /**
     * Main system for drawing an entities (block) in the language editor.
     */
    class RenderSystem : public System
    {
    public:
        RenderSystem();
        ~RenderSystem();

    protected:
        void UpdateImpl(ECSId ecsId, EntityId entityId, f32 deltaTime) override;
    };
} // namespace rpp
