#pragma once
#include "type.h"
#include "core/core.h"
#include "modules/storage.h"

namespace rpp
{
    /**
     * @brief Simple instance which is used for rendering lines.
     */
    class Line
    {
    private:
        struct LineData
        {
            u32 vertextArrayId; ///< The ID of the vertex array object (VAO) for the line.
            u32 vertexBufferId; ///< The ID of the vertex buffer object (VBO) for the line.
        };

    public:
        /**
         *  @brief The starting point of the line.
         */
        static void Initialize();

        /**
         * @brief Shutdown the Line module.
         */
        static void Shutdown();

    public:
        /**
         * @brief Create a new line instance.
         * @return The ID of the created line instance.
         */
        static u32 Create();

        /**
         * @brief Destroy a line instance.
         * @param id The ID of the line instance to destroy.
         */
        static void Draw(u32 id, const Point &start, const Point &end);

        /**
         * @brief Destroy a line instance.
         * @param id The ID of the line instance to destroy.
         */
        static void Destroy(u32 id);

    private:
        static Scope<Storage<LineData>> s_lines; ///< Storage for line instances.
    };
} // namespace rpp
