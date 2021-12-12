#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Log.h"

PerspectiveCamera::PerspectiveCamera()
    : m_position(0.0f), m_direction(0.0f), m_aspectRatio(1.0f)
{
    m_position = {3.0f, 3.0f, -2.0f};
}

void PerspectiveCamera::onWindowResize(uint32_t width, uint32_t height)
{
    m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

glm::mat4 PerspectiveCamera::getViewMatrix()
{
    return glm::lookAt(m_position, m_direction, { 0.0f, 1.0f, 0.0f });
}

glm::mat4 PerspectiveCamera::getProjectionMatrix()
{
    return glm::perspective(45.0f, m_aspectRatio, 0.0001f, 100.0f);
}