#pragma once
#include "core/core.h"
#include "glm/glm.hpp"

namespace rpp
{
    /**
     * @brief Store a 2D point with x and y coordinates.
     */
    struct RPP_JSON Point
    {
        f32 x RPP_JSON_KEY("x"); ///< The x-coordinate of the point.
        f32 y RPP_JSON_KEY("y"); ///< The y-coordinate of the point.
    };

    /**
     * @brief Store a color with red, green, blue, and alpha components in floating point precision from 0.0 to 1.0.
     */
    struct RPP_JSON Color
    {
        f32 r RPP_JSON_KEY("r"); ///< The red component of the color (0.0 to 1.0).
        f32 g RPP_JSON_KEY("g"); ///< The green component of the color (0.0 to 1.0).
        f32 b RPP_JSON_KEY("b"); ///< The blue component of the color (0.0 to 1.0).
        f32 a RPP_JSON_KEY("a"); ///< The alpha (opacity) component of the color (0.0 to 1.0).
    };

    /**
     * @brief Store a rectangle defined by its center coordinates, width, and height.
     */
    struct RPP_JSON Rect
    {
        f32 centerX RPP_JSON_KEY("x");     ///< The x-coordinate of the rectangle's center.
        f32 centerY RPP_JSON_KEY("y");     ///< The y-coordinate of the rectangle's center.
        f32 width RPP_JSON_KEY("width");   ///< The width of the rectangle.
        f32 height RPP_JSON_KEY("height"); ///< The height of the rectangle.
    };

    typedef glm::vec2 Vec2;
    typedef glm::vec3 Vec3;
    typedef glm::vec4 Vec4;

    typedef glm::ivec2 IVec2;
    typedef glm::ivec3 IVec3;
    typedef glm::ivec4 IVec4;

    typedef glm::mat2 Mat2x2;
    typedef glm::mat3 Mat3x3;
    typedef glm::mat4 Mat4x4;
} // namespace rpp
