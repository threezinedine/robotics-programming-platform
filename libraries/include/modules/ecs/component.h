#pragma once
#include "core/core.h"
#include "type.h"

namespace rpp
{

    /**
     * The data structure which holds the metadata information of a custom component.
     */
    struct Component
    {
        ComponentId id; ///< Be used for identifying the component type.
        b8 isActive;    ///< If not active, the component will not be accounted for when matching the entity to the system's requirement.
        void *pData;    ///< Be copied into the heap memory when attaching to an entity, must be deallocated manually via the ECS system.
        u32 size;       ///< The size of the `pData`.
    };
} // namespace rpp
