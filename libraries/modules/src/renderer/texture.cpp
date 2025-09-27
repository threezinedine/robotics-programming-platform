#include "modules/renderer/texture.h"
#include "platforms/platforms.h"

namespace rpp
{
    Texture::Texture(u32 width, u32 height)
        : m_width(width), m_height(height), m_textureId(0)
    {
    }

    Texture::Texture(const String &filePath)
        : m_width(0), m_height(0), m_textureId(0)
    {
    }

    Texture::Texture(const Texture &other)
        : m_width(other.m_width), m_height(other.m_height), m_textureId(other.m_textureId)
    {
    }

    Texture::~Texture()
    {
    }
} // namespace rpp
