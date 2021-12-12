#pragma once

#include <glm/glm.hpp>

class PerspectiveCamera
{
public:

    PerspectiveCamera();

    void onWindowResize(uint32_t width, uint32_t height);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

private:

    glm::vec3 m_position;
    glm::vec3 m_direction;
    float m_aspectRatio;
};