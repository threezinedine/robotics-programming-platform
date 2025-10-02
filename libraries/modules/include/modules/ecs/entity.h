#pragma once
#include "core/core.h"
#include "component.h"
#include "type.h"

namespace rpp
{
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

        /**
         * The list of component IDs attached to the entity for easy searching.
         * The index is the component id, the value is the index in the `ppComponents` array.
         * If the value is -1, the component is not attached to the entity.
         */
        ComponentId componentIds[MAX_NUMBER_OF_COMPONENTS];
    };
} // namespace rpp
