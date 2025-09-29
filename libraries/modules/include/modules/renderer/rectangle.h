#pragma once
#include "core/core.h"
#include "modules/storage.h"
#include "type.h"

namespace rpp
{
    /**
     * @brief Simple instance which is used for rendering rectangles.
     */
    class RPP_PYTHON_BINDING Rectangle
    {
    private:
        struct RectangleData
        {
            u32 rendererId;     ///< The id of the renderer associated with this rectangle.
            u32 vertexBufferId; ///< The ID of the vertex buffer in the graphics API.
            u32 vertexArrayId;  ///< The ID of the vertex array in the graphics API.
            u32 programId;      ///< The ID of the shader program used to render the rectangle.
        };

    public:
        /**
         * @brief Initialize the Rectangle storage, should be called once before using any Rectangle related features.
         */
        static void Initialize();

        /**
         * @brief Shutdown the Rectangle storage, must be called once before exiting the application.
         */
        static void Shutdown();

    public:
        /**
         * @brief Create a new rectangle instance.
         *
         * @return The ID of the created rectangle instance.
         */
        static u32 Create() RPP_PYTHON_BINDING;

        /**
         * @brief Destroy the rectangle instance and free associated resources.
         *
         * @param rectangleId The ID of the rectangle instance to destroy.
         */
        static void Destroy(u32 rectangleId) RPP_PYTHON_BINDING;

        /**
         * @brief Draw the rectangle using the associated renderer.
         * @param rectangleId The ID of the rectangle instance to draw.
         * @param rect The rectangle parameters including center coordinates, width, and height.
         */
        static void Draw(u32 rectangleId, const Rect &rect) RPP_PYTHON_BINDING;

    private:
        static Scope<Storage<RectangleData>> s_rectangles; ///< Storage for all created rectangles.
    };

} // namespace rpp
