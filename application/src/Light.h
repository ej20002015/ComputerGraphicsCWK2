#pragma once

#include <glm/glm.hpp>

class Light
{
public:
    
    struct LightSpecification
    {
        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        glm::vec4 colour = { 1.0f, 1.0f, 1.0f, 1.0f };
    };

public:

    Light() = default;
    ~Light();
    void init(const LightSpecification& specification);

    void update();

    void renderLocation();

    void setPosition(const glm::vec3& position) { m_specification.position = position; setLightParameters(); }
    void setColour(const glm::vec4& colour) { m_specification.colour = colour; setLightParameters(); }

    const glm::vec3& getPosition() const { return m_specification.position; }
    const glm::vec4& getColour() const { return m_specification.colour; }

private:

    void setLightParameters();

private:

    // Used to track the number of lights in the scene so that the legacy OpenGL limit of 8 is not exceeded
    static uint32_t s_lightCount;

    // Initially set lightNum to 8 which is an invalid value
    uint32_t m_lightNum = 8;

    LightSpecification m_specification;
};