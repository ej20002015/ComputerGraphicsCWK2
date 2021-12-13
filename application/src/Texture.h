#pragma once

#include <string>

#include <stb_image.h>
#include <glad/glad.h>

class Texture
{
public:

    enum class WrappingMode
    {
        CLAMP_TO_EDGE = 0,
        REPEAT,
        MIRRORED_REPEAT
    };

    enum class Filter
    {
        LINEAR = 0,
        NEAREST
    };

    struct TextureSpecification
    {
        std::string filepath;
        WrappingMode wrappingMode = WrappingMode::CLAMP_TO_EDGE;
        Filter minFilter = Filter::LINEAR;
        Filter magFilter = Filter::LINEAR;
    };

public:

    Texture(const TextureSpecification& specification);
    Texture() = default;
    ~Texture();

    void bind();

    uint32_t getRendererID() { return m_rendererID; }

private:

    GLenum getGLWrappingMode(WrappingMode wrappingMode);
    GLenum getGLMinFilter(Filter minFilter);
    GLenum getGLMagFilter(Filter magFilter);

private:

    uint32_t m_rendererID = 0;

};