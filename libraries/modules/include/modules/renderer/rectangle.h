#pragma once
#include "core/core.h"
#include "drawable.h"

namespace rpp
{
    /**
     * @brief Simple instance which is used for rendering rectangles.
     */
    class Rectangle : public Drawable
    {
    public:
        /**
         * @brief Constructs a unit rectangle centered at the origin (0,0) with width and height of 1.0.
         */
        Rectangle();

        /**
         * @brief Constructs a rectangle centered at (centerX, centerY) with the specified width and height.
         * @param centerX The x-coordinate of the rectangle's center.
         * @param centerY The y-coordinate of the rectangle's center.
         * @param width The width of the rectangle.
         * @param height The height of the rectangle.
         */
        Rectangle(f32 centerX, f32 centerY, f32 width, f32 height);

        Rectangle(const Rectangle &other) = delete;

        /**
         * @brief Destructs the rectangle and frees associated resources.
         */
        ~Rectangle();

        /**
         * @brief Draw the rectangle using the associated renderer.
         */
        virtual void Draw() const override;

    private:
        f32 m_centerX;        ///< The x-coordinate of the rectangle's center.
        f32 m_centerY;        ///< The y-coordinate of the rectangle's center.
        f32 m_width;          ///< The width of the rectangle.
        f32 m_height;         ///< The height of the rectangle.
        u32 m_vertexBufferId; ///< The ID of the vertex buffer in the graphics API.
        u32 m_vertexArrayId;  ///< The ID of the vertex array in the graphics API.
    };
} // namespace rpp
