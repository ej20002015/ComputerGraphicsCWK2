#include "Texture.h"

#include <filesystem>

#include "Log.h"

void Texture::init(const TextureSpecification& specification)
{
    int32_t width, height, channels;

    stbi_set_flip_vertically_on_load(true);

    void* imageData = stbi_load(specification.filepath.c_str(),  &width, &height, &channels, STBI_rgb);

    if (!imageData)
        Log::fatal("Cannot load texture " + std::filesystem::current_path().string() + specification.filepath + ": " + std::string(stbi_failure_reason()));

    m_width = static_cast<uint32_t>(width);
    m_height = static_cast<uint32_t>(height);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &m_rendererID);

    glBindTexture(GL_TEXTURE_2D, m_rendererID);

    GLenum wrappingMode = getGLWrappingMode(specification.wrappingMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrappingMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getGLMinFilter(specification.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getGLMagFilter(specification.magFilter));

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imageData);
}

Texture::~Texture()
{
    if (m_rendererID)
        glDeleteTextures(1, &m_rendererID);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_rendererID);
}

GLenum Texture::getGLWrappingMode(WrappingMode wrappingMode)
{
    switch (wrappingMode)
    {
        case WrappingMode::CLAMP_TO_EDGE:
            return GL_CLAMP_TO_EDGE;
            break;
        case WrappingMode::REPEAT:
            return GL_REPEAT;
            break;
        case WrappingMode::MIRRORED_REPEAT:
            return GL_MIRRORED_REPEAT;
            break;
        default:
            Log::fatal("Wrapping mode not recognised");
            return 0;
            break;
    }
}

GLenum Texture::getGLMinFilter(Filter minFilter)
{
    switch (minFilter)
    {
        case Filter::LINEAR:
            return GL_LINEAR_MIPMAP_LINEAR;
            break;
        case Filter::NEAREST:
            return GL_NEAREST;
            break;
        default:
            Log::fatal("Min filter not recognised");
            return 0;
            break;
    }
}

GLenum Texture::getGLMagFilter(Filter magFilter)
{
    switch (magFilter)
    {
        case Filter::LINEAR:
            return GL_LINEAR;
            break;
        case Filter::NEAREST:
            return GL_NEAREST;
            break;
        default:
            Log::fatal("Mag filter not recognised");
            return 0;
            break;
    }
}