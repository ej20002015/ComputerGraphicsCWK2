#include "Light.h"

#include <glm/gtc/type_ptr.hpp>

#include "Log.h"
#include "Renderer.h"
#include "Material.h"

uint32_t Light::s_lightCount = 0;

Light::~Light()
{
    s_lightCount--;
}

void Light::init(const LightSpecification& specification)
{
    if (s_lightCount > 7)
    {
        Log::message("lightCount: " + std::to_string(s_lightCount));
        Log::fatal("ERROR: Cannot initialise light since OpenGL only supports a maximum of 8");
        return;
    }

    m_lightNum = s_lightCount;
    m_specification = specification;

    glEnable(GL_LIGHT0 + s_lightCount);

    setLightParameters();

    s_lightCount++;
}

void Light::update()
{
    // Updates the position of the light every frame which is necessary to stop the light moving with the camera

    glm::vec4 lightPosition = glm::vec4(m_specification.position, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLightfv(GL_LIGHT0 + m_lightNum, GL_POSITION, glm::value_ptr(lightPosition));

    glPopMatrix();
}

void Light::renderLocation()
{
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(m_specification.position));
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), { 0.2f, 0.2f, 0.2f});

    Material colourMaterial = MaterialLibrary::getMaterial("WHITE_COLOUR");
    // Set the colour of the cube to the current colour of the light
    // Multiplying by 5 to overcome the ambient constant value of the scene
    colourMaterial.ambient = m_specification.colour * 5.0f;

    Renderer::drawCube(translation * scale, colourMaterial);
}

void Light::setLightParameters()
{
    glm::vec4 lightPosition = glm::vec4(m_specification.position, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLightfv(GL_LIGHT0 + m_lightNum, GL_POSITION, glm::value_ptr(lightPosition));

    glPopMatrix();

    glLightfv(GL_LIGHT0 + m_lightNum, GL_DIFFUSE, glm::value_ptr(m_specification.colour));
    glLightfv(GL_LIGHT0 + m_lightNum, GL_SPECULAR, glm::value_ptr(m_specification.colour));
}