#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

// Represents a hierarchical lumberjack model

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

    // An animation curve that goes smoothly from 0 to 1, and then smoothly back to 0 again
    // Used to animate the upper arm
    float getRotationFunctionValue(float x);
    // An animation curve that goes smoothly from 0 to 1, then smoothly dips to about 0.5, then smoothly back up to 1, and then smoothly back down to 0 again
    // Used to animate the lower arm and ax
    float getJerkyRotationFunctionValue(float x);

private:

    float m_animationSpeed = 0.5f;
    float m_currentRotationValue = 0.0f;
    glm::mat4 m_transform;

    Texture* m_axHandleTexture = nullptr;
    Texture* m_faceTexture = nullptr;
};