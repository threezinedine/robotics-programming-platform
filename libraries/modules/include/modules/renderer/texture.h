#pragma once
#include "core/core.h"

namespace rpp
{
    /**
     * @brief Object for dealing with textures in the renderer module.
     */
    class Texture
    {
    public:
        /**
         * @brief Constructs a texture with the specified width and height and all pixels initialized to zero.
         */
        Texture(u32 width, u32 height);

        /**
         * @brief Constructs a texture by loading it from the specified file path. The file format is inferred from the file extension.
         * The width, height, and pixel data are loaded from the file.
         * @param filePath The path to the texture file.
         */
        Texture(const String &filePath);

        Texture(const Texture &);

        /**
         * @brief Destructs the texture and frees associated resources.
         */
        ~Texture();

    private:
        u32 m_width;     ///< The width of the texture in pixels.
        u32 m_height;    ///< The height of the texture in pixels.
        u32 m_textureId; ///< The unique identifier for the texture in the graphics API.
    };
} // namespace rpp
