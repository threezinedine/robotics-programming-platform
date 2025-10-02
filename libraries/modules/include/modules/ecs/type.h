#pragma once
#include "core/core.h"

#define MAX_NUMBER_OF_COMPONENTS 32 ///< The maximum number of components the whole ECS system can support.

namespace rpp
{
    typedef u32 EntityId;    ///< The type used for entity IDs.
    typedef u32 ECSId;       ///< The type used for ECS system IDs.
    typedef u32 ComponentId; ///< The type used for component IDs.
    typedef u32 SystemId;    ///< The type used for system IDs.
} // namespace rpp
