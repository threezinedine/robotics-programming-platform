#pragma once
#include "core/core.h"
#include "component.h"

namespace rpp
{
    typedef u32 EntityId; ///< The type used for entity IDs.

    /**
     * The main instance of a ECS system, its contains mutliple components, if the components match the system's requirement,
     * the entity will be processed by the system each frame.
     */
    struct Entity
    {
        EntityId id;              ///< The ID of the entity.
        b8 isActive;              ///< If not active, the entity will not be processed by any system.
        Component **ppComponents; ///< The list of components attached to the entity.
        u32 numberOfComponents;   ///< The number of components attached to the entity.
    };
} // namespace rpp
