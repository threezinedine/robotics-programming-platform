#pragma once
#include "core/core.h"

namespace rpp
{
    /**
     * @brief Store a 2D point with x and y coordinates.
     */
    struct RPP_JSON RPP_PYTHON_BINDING Point
    {
        f32 x RPP_JSON_KEY("x"); ///< The x-coordinate of the point.
        f32 y RPP_JSON_KEY("y"); ///< The y-coordinate of the point.
    };

    /**
     * @brief Store a color with red, green, blue, and alpha components in floating point precision from 0.0 to 1.0.
     */
    struct RPP_JSON RPP_PYTHON_BINDING Color
    {
        f32 r RPP_JSON_KEY("r"); ///< The red component of the color (0.0 to 1.0).
        f32 g RPP_JSON_KEY("g"); ///< The green component of the color (0.0 to 1.0).
        f32 b RPP_JSON_KEY("b"); ///< The blue component of the color (0.0 to 1.0).
        f32 a RPP_JSON_KEY("a"); ///< The alpha (opacity) component of the color (0.0 to 1.0).
    };
} // namespace rpp
