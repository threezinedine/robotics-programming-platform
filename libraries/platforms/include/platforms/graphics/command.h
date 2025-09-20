#pragma once
#include "platforms/common.h"

namespace rpp
{
    /**
     * @brief All types of actions that the graphics backend can perform.
     */
    enum class GraphicsCommandType
    {
        CLEAR_COLOR,  ///< Specify the screen color.
        SWAP_BUFFERS, ///< Swap the front and back buffers.
        COUNT,
    };

    /**
     * @brief The data structure for a graphics command which will be send to the graphics backend. (platform queue)
     */
    struct GraphicsCommandData
    {
        GraphicsCommandType type; ///< The type of the command.
        void *data;               ///< The data of the command.
    };

    struct ClearColorCommandData
    {
        f32 color[4]; ///< The RGBA color values.
    };
} // namespace rpp
