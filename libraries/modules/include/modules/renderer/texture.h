#pragma once
#include "core/core.h"
#include "modules/storage.h"

namespace rpp
{
    /**
     * @brief Object for dealing with textures in the renderer module.
     */
    class Texture
    {
    private:
        struct TextureData
        {
            u32 rendererId; ///< The ID of the renderer that created this texture.
            u32 width;      ///< The width of the texture in pixels.
            u32 height;     ///< The height of the texture in pixels.
            u32 textureId;  ///< The unique identifier for the texture in the graphics API
            u32 slot;       ///< The texture slot (for multi-texturing).
        };

    public:
        /**
         * @brief Initializes the texture system. Must be called before creating any Texture objects.
         */
        static void Initialize();

        /**
         * @brief Shuts down the texture system and frees any global resources. Should be called when textures are no longer needed.
         */
        static void Shutdown();

    public:
        /**
         * @brief Constructs a texture by loading it from the specified file path. The file format is inferred from the file extension.
         * The width, height, and pixel data are loaded from the file.
         * @param filePath The path to the texture file.
         */
        static u32 Create(const String &filePath);

        /**
         * @brief Destroys the texture with the specified ID, freeing its resources.
         */
        static void Destroy(u32 textureId);

        /**
         * @brief Activates the texture with the specified ID on the given texture slot.
         * @param textureId The ID of the texture to activate.
         * @param slot The texture slot to activate the texture on (e.g., 0 for GL_TEXTURE0).
         * @param program The shader program that will use this texture. The program must be active (used) before calling this method.
         */
        static void Activate(u32 textureId, u32 slot);

    private:
        static Scope<Storage<TextureData>> s_textureStorage; ///< Storage for managing texture data.
    };
} // namespace rpp
