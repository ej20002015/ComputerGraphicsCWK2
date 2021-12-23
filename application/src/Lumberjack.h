#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

class Lumberjack
{
public:
    
    Lumberjack() : m_transform(1.0f) {}

    void update(float timeStep);

    void setAnimationSpeed(float animationSpeed) { m_animationSpeed = animationSpeed; }
    void setTransform(const glm::mat4& transform) { m_transform = transform; }
    void setAxHandleTexture(Texture* axHandleTexture) { m_axHandleTexture = axHandleTexture; }
    void setFaceTexture(Texture* faceTexture) { m_faceTexture = faceTexture; }

    float getAnimationSpeed() const { return m_animationSpeed; }

private:

    float getRotationFunctionValue(float x);
    float getJerkyRotationFunctionValue(float x);

private:

    float m_animationSpeed = 0.5f;
    float m_currentRotationValue = 0.0f;
    glm::mat4 m_transform;

    Texture* m_axHandleTexture = nullptr;
    Texture* m_faceTexture = nullptr;
};