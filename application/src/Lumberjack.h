#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

class Lumberjack
{
public:
    
    Lumberjack() : m_transform(1.0f) {}

    void update(float timeStep);

    void setRotationSpeed(float rotationSpeed) { m_rotationSpeed = rotationSpeed; }
    void setTransform(const glm::mat4& transform) { m_transform = transform; }
    void setFaceTexture(Texture* faceTexture) { m_faceTexture = faceTexture; }

    float getRotationSpeed() const { return m_rotationSpeed; }

private:

    float getRotationFunctionValue(float x);
    float getJerkyRotationFunctionValue(float x);

private:

    float m_rotationSpeed = 0.5f;
    Texture* m_faceTexture = nullptr;
    glm::mat4 m_transform;
};