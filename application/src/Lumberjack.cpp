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

    // m_currentRotationValue goes smoothly from 0 to 1, and then straight back to 0 again
    // Is used as input into animation curves that return values that can be used to calculate the current rotation of limbs in the model
    m_currentRotationValue = glm::mod<float>(m_currentRotationValue + (m_animationSpeed * timeStep), 1.0f);

    glm::mat4 lumberjackCenterPosition = glm::translate(glm::mat4(1.0f), { 0.0f, 0.8f, 0.0f });
    glMultMatrixf(glm::value_ptr(lumberjackCenterPosition));
    glPushMatrix();

    // Draw torso
    Renderer::drawCube(glm::scale(glm::mat4(1.0f), { 0.35f, 0.45f, 0.15f }), MaterialLibrary::getMaterial("RED_CLOTH"));

    // Draw legs

    glm::mat4 lumberjackLegPosition = glm::translate(glm::mat4(1.0f), { 0.0f, -0.225f, 0.0f });
    glMultMatrixf(glm::value_ptr(lumberjackLegPosition));
    glPushMatrix();

    glm::mat4 legRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
    glm::mat4 legScale = glm::scale(glm::mat4(1.0f), { 0.1f, 0.1f, 0.4f });

    glm::mat4 legLeftTranslation = glm::translate(glm::mat4(1.0f), { (-0.35f / 2.0f) + 0.1f, -0.1f, 0.0f });
    glm::mat4 legRightTranslation = glm::translate(glm::mat4(1.0f), { (0.35f / 2.0f) - 0.1f, -0.1f, 0.0f });

    Renderer::drawCylinder(legLeftTranslation * legRotation * legScale, MaterialLibrary::getMaterial("BLUE_CLOTH"));
    Renderer::drawCylinder(legRightTranslation * legRotation * legScale, MaterialLibrary::getMaterial("BLUE_CLOTH"));

    // Draw boots

    glm::mat4 footScale = glm::scale(glm::mat4(1.0f), { 0.12f, 0.05f , 0.2f });

    glm::mat4 footTranslationLeft = glm::translate(glm::mat4(1.0f), { (-0.35f / 2.0f) + 0.1f, -0.285f, 0.03f });
    glm::mat4 footTranslationRight = glm::translate(glm::mat4(1.0f), { (0.35f / 2.0f) - 0.1f, -0.285f, 0.03f });

    Renderer::drawCube(footTranslationLeft * footScale, MaterialLibrary::getMaterial("BLACK_LEATHER"));
    Renderer::drawCube(footTranslationRight * footScale, MaterialLibrary::getMaterial("BLACK_LEATHER"));

    glPopMatrix();
    glPopMatrix();

    // Draw neck

    glm::mat4 neckTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, 0.25f, 0.0f });
    glMultMatrixf(glm::value_ptr(neckTranslation));
    glPushMatrix();

    glm::mat4 neckRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
    glm::mat4 neckScale = glm::scale(glm::mat4(1.0f), { 0.1f, 0.1f, 0.1f });

    Renderer::drawCylinder(neckRotation * neckScale, MaterialLibrary::getMaterial("SKIN"));

    // Draw head with provided texture

    glm::mat4 headTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, 0.13f, 0.0f });
    glm::mat4 headScale = glm::scale(glm::mat4(1.0f), { 0.2f, 0.2f, 0.2f });

    Renderer::CubeTextureSpecification headCubeTextureSpecification;
    headCubeTextureSpecification.frontFace = m_faceTexture;
    Renderer::drawCube(headTranslation * headScale, MaterialLibrary::getMaterial("SKIN"), headCubeTextureSpecification);

    // Draw shoulders

    glm::mat4 shouldersTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, -0.1f, 0.0f });
    glMultMatrixf(glm::value_ptr(shouldersTranslation));
    glPushMatrix();

    glm::mat4 shouldersRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 0.0f, 1.0f, 0.0f });
    glm::mat4 shouldersScale = glm::scale(glm::mat4(1.0f), { 0.1f, 0.1f, 0.5f });

    Renderer::drawCylinder(shouldersRotation * shouldersScale, MaterialLibrary::getMaterial("RED_CLOTH"));

    // Draw upper, lower arms and the ax

    // Upper left arm

    const float upperArmRotationAngleApex = 55.0f;
    float currentUpperArmRotationAngle = getRotationFunctionValue(m_currentRotationValue) * upperArmRotationAngleApex;
    glm::mat4 upperArmRotation = glm::rotate(glm::mat4(1.0f), glm::radians(currentUpperArmRotationAngle), { -1.0f, 0.0f, 0.0f });
    glMultMatrixf(glm::value_ptr(upperArmRotation));
    glPushMatrix();

    glm::mat4 upperArmScale = glm::scale(glm::mat4(1.0f), { 0.08f, 0.22f, 0.11f });

    glm::mat4 upperArmLeftTranslation = glm::translate(glm::mat4(1.0f), { -0.25f, -0.06f, 0.0f });
    glMultMatrixf(glm::value_ptr(upperArmLeftTranslation));
    glPushMatrix();

    Renderer::drawCube(upperArmScale, MaterialLibrary::getMaterial("RED_CLOTH"));

    // Lower left arm

    glm::mat4 lowerArmTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, -0.21f, 0.0f });
    glMultMatrixf(glm::value_ptr(lowerArmTranslation));
    // Translate to move the arm around a certian point
    glMultMatrixf(glm::value_ptr(glm::translate(glm::mat4(1.0f), { 0.0f, 0.08f, 0.0f })));
    glPushMatrix();

    const float lowerArmRotationAngleApex = 70.0f;
    float currentLowerArmJerkRotationAngle = getJerkyRotationFunctionValue(m_currentRotationValue) * lowerArmRotationAngleApex;
    glm::mat4 lowerArmAndHammerRotation = glm::rotate(glm::mat4(1.0f), glm::radians(currentLowerArmJerkRotationAngle), { -1.0f, 0.0f, 0.0f });
    glMultMatrixf(glm::value_ptr(lowerArmAndHammerRotation));
    // Move arm back to correct position
    glMultMatrixf(glm::value_ptr(glm::translate(glm::mat4(1.0f), { 0.0f, -0.08f, 0.0f })));
    glPushMatrix();

    glm::mat4 lowerArmRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
    glm::mat4 lowerArmScale = glm::scale(glm::mat4(1.0f), { 0.07f, 0.08f, 0.28f });

    Renderer::drawCylinder(lowerArmRotation * lowerArmScale, MaterialLibrary::getMaterial("SKIN"));

    // Ax handle

    glm::mat4 axHandleTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, -0.1f, 0.1f });
    glMultMatrixf(glm::value_ptr(axHandleTranslation));
    glPushMatrix();

    glm::mat4 axHandleScale = glm::scale(glm::mat4(1.0f), { 0.05f, 0.05f, 0.2f });

    Renderer::CylinderTextureSpecification axHandleCylinderTextureSpecification(m_axHandleTexture);
    Renderer::drawCylinder(axHandleScale, MaterialLibrary::getMaterial("WOOD"), axHandleCylinderTextureSpecification);

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

    Renderer::drawCube(upperArmScale, MaterialLibrary::getMaterial("RED_CLOTH"));

    // Lower right arm

    glMultMatrixf(glm::value_ptr(lowerArmTranslation));

    Renderer::drawCylinder(lowerArmRotation * lowerArmScale, MaterialLibrary::getMaterial("SKIN"));

    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}

#include "Log.h"

float Lumberjack::getRotationFunctionValue(float x)
{
    return (-1.0f * glm::pow(((2.0f * x) - 1.0f), 2.0f)) + 1.0f;
}

float Lumberjack::getJerkyRotationFunctionValue(float x)
{
    float func1 = (-1.0309f * glm::pow(((2.0f * x) - 1.0f), 2.0f)) + 1.0309f;
    float func2 = glm::min<float>(0.0f, 10.0f * glm::pow((2.0f * x) - 1.0f, 2.0f) - 0.3f);
    return func1 + (2 * func2);
}