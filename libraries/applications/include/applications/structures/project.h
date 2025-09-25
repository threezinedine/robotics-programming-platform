#pragma once
#include "core/core.h"

namespace rpp
{
    /**
     * @brief The needed information which are used for creating a project object. This is used in Python binding and JSON mapping.
     */
    struct RPP_JSON ProjectDescription
    {
        String name RPP_JSON_KEY("name"); ///< The name of the project.
    };
} // namespace rpp
