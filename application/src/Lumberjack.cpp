#include "Lumberjack.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"

void Lumberjack::update(float timeStep)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glMultMatrixf(glm::value_ptr(m_transform));

    static float currentRotationValue = 0.0f;
    currentRotationValue = glm::mod<float>(currentRotationValue + (m_rotationSpeed * timeStep), 1.0f);

    glm::mat4 lumberjackCenterPosition = glm::translate(glm::mat4(1.0f), { 0.0f, 0.8f, 0.0f });
    glMultMatrixf(glm::value_ptr(lumberjackCenterPosition));
    glPushMatrix();

    // Draw torso
    Renderer::drawCube(glm::scale(glm::mat4(1.0f), { 0.35f, 0.45f, 0.15f }), MaterialLibrary::getMaterial("GREEN_PLASTIC"));

    // Draw legs

    glm::mat4 lumberjackLegPosition = glm::translate(glm::mat4(1.0f), { 0.0f, -0.225f, 0.0f });
    glMultMatrixf(glm::value_ptr(lumberjackLegPosition));
    glPushMatrix();

    glm::mat4 legRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
    glm::mat4 legScale = glm::scale(glm::mat4(1.0f), { 0.1f, 0.1f, 0.4f });

    glm::mat4 legLeftTranslation = glm::translate(glm::mat4(1.0f), { (-0.35f / 2.0f) + 0.1f, -0.1f, 0.0f });
    glm::mat4 legRightTranslation = glm::translate(glm::mat4(1.0f), { (0.35f / 2.0f) - 0.1f, -0.1f, 0.0f });

    Renderer::drawCylinder(legLeftTranslation * legRotation * legScale, MaterialLibrary::getMaterial("GREEN_PLASTIC"));
    Renderer::drawCylinder(legRightTranslation * legRotation * legScale, MaterialLibrary::getMaterial("GREEN_PLASTIC"));

    // Draw boots

    glm::mat4 footScale = glm::scale(glm::mat4(1.0f), { 0.1f, 0.025f , 0.15f });

    glm::mat4 footTranslationLeft = glm::translate(glm::mat4(1.0f), { (-0.35f / 2.0f) + 0.1f, -0.285f, 0.1f });
    glm::mat4 footTranslationRight = glm::translate(glm::mat4(1.0f), { (0.35f / 2.0f) - 0.1f, -0.285f, 0.1f });

    Renderer::drawCube(footTranslationLeft * footScale, MaterialLibrary::getMaterial("GREEN_PLASTIC"));
    Renderer::drawCube(footTranslationRight * footScale, MaterialLibrary::getMaterial("GREEN_PLASTIC"));

    glPopMatrix();
    glPopMatrix();

    // Draw neck

    glm::mat4 neckTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, 0.25f, 0.0f });
    glMultMatrixf(glm::value_ptr(neckTranslation));
    glPushMatrix();

    glm::mat4 neckRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
    glm::mat4 neckScale = glm::scale(glm::mat4(1.0f), { 0.1f, 0.1f, 0.1f });

    Renderer::drawCylinder(neckRotation * neckScale, MaterialLibrary::getMaterial("GREEN_PLASTIC"));

    // Draw head with provided texture

    glm::mat4 headTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, 0.13f, 0.0f });
    glm::mat4 headScale = glm::scale(glm::mat4(1.0f), { 0.2f, 0.2f, 0.2f });

    Renderer::CubeTextureSpecification headCubeTextureSpecification;
    headCubeTextureSpecification.frontFace = m_faceTexture;
    Renderer::drawCube(headTranslation * headScale, MaterialLibrary::getMaterial("GREEN_PLASTIC"), headCubeTextureSpecification);

    // Draw shoulders

    glm::mat4 shouldersTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, -0.1f, 0.0f });
    glMultMatrixf(glm::value_ptr(shouldersTranslation));
    glPushMatrix();

    glm::mat4 shouldersRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 0.0f, 1.0f, 0.0f });
    glm::mat4 shouldersScale = glm::scale(glm::mat4(1.0f), { 0.1f, 0.1f, 0.5f });

    Renderer::drawCylinder(shouldersRotation * shouldersScale, MaterialLibrary::getMaterial("GREEN_PLASTIC"));

    // Draw upper, lower arms and the ax

    // Upper left arm

    const float upperArmRotationAngleApex = 45.0f;
    float currentUpperArmRotationAngle = getRotationFunctionValue(currentRotationValue) * upperArmRotationAngleApex;
    glm::mat4 upperArmRotation = glm::rotate(glm::mat4(1.0f), glm::radians(currentUpperArmRotationAngle), { -1.0f, 0.0f, 0.0f });
    glMultMatrixf(glm::value_ptr(upperArmRotation));
    glPushMatrix();

    glm::mat4 upperArmScale = glm::scale(glm::mat4(1.0f), { 0.08f, 0.22f, 0.11f });

    glm::mat4 upperArmLeftTranslation = glm::translate(glm::mat4(1.0f), { -0.25f, -0.06f, 0.0f });
    glMultMatrixf(glm::value_ptr(upperArmLeftTranslation));
    glPushMatrix();

    Renderer::drawCube(upperArmScale, MaterialLibrary::getMaterial("GREEN_PLASTIC"));

    // Lower left arm

    const float lowerArmRotationAngleApex = 70.0f;
    float currentLowerArmJerkRotationAngle = getJerkyRotationFunctionValue(currentRotationValue) * lowerArmRotationAngleApex;
    glm::mat4 lowerArmAndHammerRotation = glm::rotate(glm::mat4(1.0f), glm::radians(currentLowerArmJerkRotationAngle), { -1.0f, 0.0f, 0.0f });
    glMultMatrixf(glm::value_ptr(lowerArmAndHammerRotation));
    glPushMatrix();

    glm::mat4 lowerArmTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, -0.21f, 0.0f });
    glm::mat4 lowerArmRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
    glm::mat4 lowerArmScale = glm::scale(glm::mat4(1.0f), { 0.08f, 0.08f, 0.28f });

    glMultMatrixf(glm::value_ptr(lowerArmTranslation));
    glPushMatrix();

    Renderer::drawCylinder(lowerArmRotation * lowerArmScale, MaterialLibrary::getMaterial("GREEN_PLASTIC"));

    // Ax handle

    glm::mat4 axHandleTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, -0.1f, 0.1f });
    glMultMatrixf(glm::value_ptr(axHandleTranslation));
    glPushMatrix();

    glm::mat4 axHandleScale = glm::scale(glm::mat4(1.0f), { 0.05f, 0.05f, 0.2f });

    Renderer::drawCylinder(axHandleScale, MaterialLibrary::getMaterial("WOOD"));

    // Ax blade

    glm::mat4 axBladeTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, -0.05f, 0.05f });
    glm::mat4 axBladeScale = glm::scale(glm::mat4(1.0f), { 0.01f, 0.1f, 0.05f });

    Renderer::drawCube(axBladeTranslation * axBladeScale, MaterialLibrary::getMaterial("CHROME"));

    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    // Upper right arm

    glm::mat4 upperArmRightTranslation = glm::translate(glm::mat4(1.0f), { 0.25f, -0.06f, 0.0f });
    glMultMatrixf(glm::value_ptr(upperArmRightTranslation));
    glPushMatrix();

    Renderer::drawCube(upperArmScale, MaterialLibrary::getMaterial("GREEN_PLASTIC"));

    // Lower right arm

    glMultMatrixf(glm::value_ptr(lowerArmTranslation));

    Renderer::drawCylinder(lowerArmRotation * lowerArmScale, MaterialLibrary::getMaterial("GREEN_PLASTIC"));

    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}

float Lumberjack::getRotationFunctionValue(float x)
{
    return (-1.25f * glm::pow(((2.0f * x) - 1.0f), 2.0f)) + 1.25f;
}

float Lumberjack::getJerkyRotationFunctionValue(float x)
{
    float func1 = (-1.0309f * glm::pow(((2.0f * x) - 1.0f), 2.0f)) + 1.0309f;
    float func2 = glm::min<float>(0.0f, 10.0f * glm::pow((2.0f * x) - 1.0f, 2.0f) - 0.3f);
    return func1 + (2 * func2);
}