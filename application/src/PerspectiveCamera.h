#pragma once

#include <glm/glm.hpp>

class PerspectiveCamera
{
public:

    PerspectiveCamera();
    void init(uint32_t windowWidth, uint32_t windowHeight);

    void onUpdate(float timeStep);

    void onWindowResize(uint32_t width, uint32_t height);

    void setLockYAxisPosition(bool lockYAxisPosition) { m_lockYAxisPosition = lockYAxisPosition; }
    void setSpeed(float speed) { m_speed = speed; }
    void setSensitivity(float sensitivity) { m_sensitivity = sensitivity; }

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    bool getCameraActive() const { return m_cameraActive; }
    bool getLockYAxisPosition() const { return m_lockYAxisPosition; }
    float getSpeed() const { return m_speed; }
    float getSensitivity() const { return m_sensitivity; }

private:

    const glm::vec3 UP_VECTOR;

    glm::vec3 m_position;
    glm::vec3 m_viewDirection;
    glm::vec2 m_lastMousePosition;
    float m_pitch = 0.0f;
    float m_yaw = -90.0f;

    float m_aspectRatio = 1.0f;
    float m_nearClip = 0.01f;
    float m_farClip = 100.0f;
    float m_FOV = 45.0f;

    float m_speed = 2.0f;
    float m_sensitivity = 0.2f;

    // Flag that prevents a sudden change of direction when 
    // starting to move the camera every time the right mouse button is pressed
    bool m_initialCameraManipulation = true;

    bool m_cameraActive = false;
    bool m_lockYAxisPosition = false;
};