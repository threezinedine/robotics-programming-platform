#include "modules/renderer/texture.h"
#include "platforms/platforms.h"
#include "modules/renderer/renderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "core/stb_image.h"
#include "modules/renderer/program.h"

namespace rpp
{
    Scope<Storage<Texture::TextureData>> Texture::s_textureStorage = nullptr;

    void Texture::Initialize()
    {
        RPP_ASSERT(s_textureStorage == nullptr);

        auto TextureDeallocator = [](Texture::TextureData *data)
        {
            RPP_ASSERT(data != nullptr);
            RPP_ASSERT(data->rendererId != INVALID_ID);

            Renderer::Activate(data->rendererId);

            DeleteTextureCommandData deleteTexture{};
            deleteTexture.textureId = data->textureId;
            GraphicsCommandData command{GraphicsCommandType::DELETE_TEXTURE, &deleteTexture};
            Renderer::GetWindow()->ExecuteCommand(command);

            RPP_DELETE(data);
        };

        s_textureStorage = CreateScope<Storage<TextureData>>(TextureDeallocator);
    }

    void Texture::Shutdown()
    {
        RPP_ASSERT(s_textureStorage != nullptr);

        s_textureStorage.reset();
    }

    u32 Texture::Create(const String &filePath)
    {
        RPP_ASSERT(s_textureStorage != nullptr);
        u32 id = s_textureStorage->Create();
        TextureData *textureData = s_textureStorage->Get(id);

        i32 channels = 0;
        u8 *data = stbi_load(filePath.CStr(), reinterpret_cast<i32 *>(&textureData->width), reinterpret_cast<i32 *>(&textureData->height), &channels, 4);
        if (!data)
        {
            RPP_LOG_ERROR("Failed to load texture from file: {}", filePath);
            return INVALID_ID;
        }

        textureData->rendererId = Renderer::GetCurrentRendererId();

        CreateTextureCommandData createTexture{};
        createTexture.width = textureData->width;
        createTexture.height = textureData->height;
        createTexture.numberOfChannels = channels;
        createTexture.pPixelData = data;
        createTexture.pTextureId = &textureData->textureId;

        GraphicsCommandData command{GraphicsCommandType::CREATE_TEXTURE, &createTexture};
        Renderer::GetWindow()->ExecuteCommand(command);

        stbi_image_free(data);

        return id;
    }

    u32 Texture::Create(u32 width, u32 height, u32 channels, u8 *pData)
    {
        RPP_ASSERT(s_textureStorage != nullptr);
        u32 id = s_textureStorage->Create();
        TextureData *textureData = s_textureStorage->Get(id);

        textureData->rendererId = Renderer::GetCurrentRendererId();

        textureData->width = width;
        textureData->height = height;

        CreateTextureCommandData createTexture{};
        createTexture.width = textureData->width;
        createTexture.height = textureData->height;
        createTexture.numberOfChannels = channels;
        createTexture.pPixelData = pData;
        createTexture.pTextureId = &textureData->textureId;

        GraphicsCommandData command{GraphicsCommandType::CREATE_TEXTURE, &createTexture};
        Renderer::GetWindow()->ExecuteCommand(command);

        return id;
    }

    void Texture::Destroy(u32 textureId)
    {
        RPP_ASSERT(s_textureStorage != nullptr);
        s_textureStorage->Free(textureId);
    }

    void Texture::Activate(u32 textureId, const String &name, u32 slot)
    {
        RPP_ASSERT(s_textureStorage != nullptr);
        TextureData *textureData = s_textureStorage->Get(textureId);
        RPP_ASSERT(textureData != nullptr);
        RPP_ASSERT(textureData->rendererId == Renderer::GetCurrentRendererId());

        ActiveTextureCommandData activeTexture{};
        activeTexture.slot = slot;
        activeTexture.textureId = textureData->textureId;

        GraphicsCommandData command{GraphicsCommandType::ACTIVE_TEXTURE, &activeTexture};
        Renderer::GetWindow()->ExecuteCommand(command);

        Program::SetUniform(name, (i32)slot);
    }
} // namespace rpp
