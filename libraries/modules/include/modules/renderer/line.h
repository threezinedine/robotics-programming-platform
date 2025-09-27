#pragma once
#include "drawable.h"
#include "type.h"
#include "core/core.h"

namespace rpp
{
    /**
     * @brief Simple instance which is used for rendering lines.
     */
    class RPP_PYTHON_BINDING Line : public Drawable
    {
    public:
        Line();
        Line(const Point &start, const Point &end);
        Line(const Point &start, const Point &end, const Color &color);
        Line(const Line &other) = delete;
        ~Line();

        virtual void Draw() const override RPP_PYTHON_BINDING;

    private:
        u32 m_vertexBufferId; ///< The ID of the vertex buffer in the graphics API.
        u32 m_vertexArrayId;  ///< The ID of the vertex array in the graphics API.
    };
} // namespace rpp
