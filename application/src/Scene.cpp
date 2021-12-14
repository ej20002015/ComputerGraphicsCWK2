#include "Scene.h"

//TODO: Temp
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "Renderer.h"
#include "Application.h"

void Scene::init()
{
    uint32_t windowWidth = Application::getWindow().getWindowProperties().width;
    uint32_t windowHeight = Application::getWindow().getWindowProperties().height;
    m_camera.init(windowWidth, windowHeight);

    setUpLights();
    setUpMaterialLibrary();

    Texture::TextureSpecification testTexturespecification;
    testTexturespecification.wrappingMode = Texture::WrappingMode::REPEAT;
    testTexturespecification.filepath = "assets/textures/markus.jpg";
    m_textureTest.init(testTexturespecification);

    Texture::TextureSpecification grassTextureSpecfication;
    grassTextureSpecfication.wrappingMode = Texture::WrappingMode::REPEAT;
    grassTextureSpecfication.filepath = "assets/textures/grass.jpg";
    m_grassTexture.init(grassTextureSpecfication);
}

void Scene::shutdown()
{
}

void Scene::onUpdate(float timeStep)
{
    Renderer::setClearColour(glm::vec4(0.396f, 0.761f, 0.961f, 1.0f));

    // If the camera is being used then disable the cursor
    if (m_camera.getCameraActive() && !Application::getWindow().getWindowCursorDisabled())
        Application::getWindow().setWindowCursorDisabled(true);
    else if (!m_camera.getCameraActive() && Application::getWindow().getWindowCursorDisabled())
        Application::getWindow().setWindowCursorDisabled(false);

    // Update the camera
    m_camera.onUpdate(timeStep);

    // Set view and projection matrices
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(m_camera.getViewMatrix()));

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(m_camera.getProjectionMatrix()));

    // Render some ground
    Renderer::CubeTextureSpecification textureSpecificationGround;
    textureSpecificationGround.topFace = &m_grassTexture;
    Renderer::drawCube(glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 0.5f, 50.0f)), m_materialLibrary.at("NO_MATERIAL"), textureSpecificationGround, 50.0f);


    // Render a cube
    Renderer::drawCube(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)), m_materialLibrary.at("GOLD"));

    // Render a textured cube

    Renderer::CubeTextureSpecification textureSpecification;
    textureSpecification.frontFace = &m_textureTest;
    textureSpecification.backFace = &m_textureTest;
    textureSpecification.rightFace = &m_textureTest;
    textureSpecification.leftFace = &m_textureTest;
    textureSpecification.topFace = &m_textureTest;
    textureSpecification.bottomFace = &m_textureTest;
    Renderer::drawCube(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 0.0f)), m_materialLibrary.at("GOLD"), textureSpecification);
}

void Scene::onUIRender()
{
    ImGui::Begin("Hello World!");
    ImGui::Text("Hello World!");

    static bool smoothShading = true;
    if (ImGui::Checkbox("Smooth", &smoothShading))
    {
        if (smoothShading)
            glShadeModel(GL_SMOOTH);
        else
            glShadeModel(GL_FLAT);
    }

    ImGui::End();
}

void Scene::onWindowResize(uint32_t width, uint32_t height)
{
    m_camera.onWindowResize(width, height);
}

void Scene::setUpLights()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glLoadIdentity();

    glEnable(GL_LIGHT0);
    glm::vec4 lightPosition(2.0f, 2.0f, 0.0f, 1.0f);

    glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(lightPosition));
    glPopMatrix();
}

void Scene::setUpMaterialLibrary()
{
    m_materialLibrary["WHITE_SHINY"] =
    {
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        100.0f
    };

    m_materialLibrary["BLACK_PLASTIC"] =
    {
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.001f, 0.001f, 0.001f, 1.0f },
        { 0.5f, 0.5f, 0.5f, 1.0f },
        32.0f
    };

    m_materialLibrary["GOLD"] =
    {
        { 0.24725f, 0.1995f, 0.0745f, 1.0f },
        { 0.75164f, 0.60648f, 0.22648f, 1.0f },
        { 0.628281f, 0.555802f, 0.366065f, 1.0f },
        51.2f
    };

    m_materialLibrary["NO_MATERIAL"] =
    {
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        0.0f
    };
}
