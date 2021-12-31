#include "Texture.h"

#include <filesystem>

#include "Log.h"
#include "Renderer.h"

void Texture::init(const TextureSpecification& specification)
{
    int32_t width, height, channels;

    // By default STBI uses 0 in the y-axis to be the top of the image (like DirectX). OpenGL needs 0 to be aligned to the bottom of the image.
    // Tells stbi to flip the image when loading so it matches with what OpenGL expects
    stbi_set_flip_vertically_on_load(true);

    void* imageData = stbi_load(specification.filepath.c_str(),  &width, &height, &channels, STBI_rgb);

    if (!imageData)
        Log::fatal("Cannot load texture " + std::filesystem::current_path().string() + specification.filepath + ": " + std::string(stbi_failure_reason()));

    m_width = static_cast<uint32_t>(width);
    m_height = static_cast<uint32_t>(height);

    glGenTextures(1, &m_rendererID);

    glBindTexture(GL_TEXTURE_2D, m_rendererID);

    // Get OpenGL to do gamma correction when sampling the texture by specifying am internal format of SRGB
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

    GLenum wrappingMode = getGLWrappingMode(specification.wrappingMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getGLMagFilter(specification.magFilter));

    // Use mipmaps if supported by the version of OpenGL. If not, just use bilinear or nearest filtering.
    if (Renderer::getOpenGLMajorVersion() > 2)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getGLMinFilter(specification.minFilter));
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, specification.minFilter == Filter::LINEAR ? GL_LINEAR : GL_NEAREST);

    // Use anisotropic filtering if OpenGL version greater than 4.5 (it became a core feature in 4.6)
    if (Renderer::getOpenGLMajorVersion() > 3 && (Renderer::getOpenGLMajorVersion() > 4 || Renderer::getOpenGLMinorVersion() > 5))
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16);

    // Given the image data to the GPU so don't need it in CPU memory anymore so can tell STBI to free it
    stbi_image_free(imageData);
}

Texture::~Texture()
{
    if (m_rendererID)
        glDeleteTextures(1, &m_rendererID);
}

void Texture::bind() const
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