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

    Texture() = default;
    ~Texture();
    void init(const TextureSpecification& specification);

    void bind();

    uint32_t getRendererID() { return m_rendererID; }
    uint32_t getWidth() { return m_width; }
    uint32_t getHeight() { return m_height; }

private:

    GLenum getGLWrappingMode(WrappingMode wrappingMode);
    GLenum getGLMinFilter(Filter minFilter);
    GLenum getGLMagFilter(Filter magFilter);

private:

    uint32_t m_rendererID = 0;
    uint32_t m_width = 0, m_height = 0;

};