#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Log.h"
#include "Input.h"

PerspectiveCamera::PerspectiveCamera()
    : UP_VECTOR(0.0f, 1.0f, 0.0f), m_position(0.0f, 1.0f, 3.0f), m_viewDirection(0.0f, 0.0f, -1.0f), m_lastMousePosition(0.0f) {}

void PerspectiveCamera::init(uint32_t windowWidth, uint32_t windowHeight)
{
    m_lastMousePosition = Input::getMousePosition();
    onWindowResize(windowWidth, windowHeight);
}

void PerspectiveCamera::onUpdate(float timeStep)
{
    // Process mouse input

    // Only modify the camera when the right mouse button is being pressed
    if (!Input::isMouseButtonPressed(MouseButtonCode::MOUSE_BUTTON_2))
    {
        m_initialCameraManipulation = true;
        m_cameraActive = false;
        return;
    }

    m_cameraActive = true;

    glm::vec2 currentMousePosition = Input::getMousePosition();
    glm::vec2 delta = m_initialCameraManipulation ? glm::vec2(0.0f) : currentMousePosition - m_lastMousePosition;
    m_initialCameraManipulation = false;
    m_lastMousePosition = currentMousePosition;

    m_yaw += delta.x * m_sensitivity;
    // Invert pitch
    m_pitch -= delta.y * m_sensitivity;

    // Prevent camera from flipping
    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    else if (m_pitch < -89.0f)
        m_pitch = -89.0f;


    glm::vec3 direction;
    direction.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    direction.y = glm::sin(glm::radians(m_pitch));
    direction.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_viewDirection = glm::normalize(direction);

    // Process key inputs

    glm::vec3 rightDirection = glm::cross(m_viewDirection, UP_VECTOR);

    float currentYPosition = m_position.y;

    if (Input::isKeyPressed(KeyCode::KEY_W))
    {
        m_position += m_viewDirection * m_speed * timeStep;
    }
    else if (Input::isKeyPressed(KeyCode::KEY_S))
    {
        m_position += -m_viewDirection * m_speed * timeStep;
    }

    if (Input::isKeyPressed(KeyCode::KEY_D))
    {
        m_position += rightDirection * m_speed * timeStep;
    }
    else if (Input::isKeyPressed(KeyCode::KEY_A))
    {
        m_position += -rightDirection * m_speed * timeStep;
    }

    if (m_lockYAxisPosition)
        m_position.y = currentYPosition;
}

void PerspectiveCamera::onWindowResize(uint32_t width, uint32_t height)
{
    m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

glm::mat4 PerspectiveCamera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_viewDirection, UP_VECTOR);
}

glm::mat4 PerspectiveCamera::getProjectionMatrix() const
{
    return glm::perspective(m_FOV, m_aspectRatio, m_nearClip, m_farClip);
}