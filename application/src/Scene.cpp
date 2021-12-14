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

    Texture::TextureSpecification specification;
    specification.wrappingMode = Texture::WrappingMode::REPEAT;
    specification.filepath = "assets/textures/markus.jpg";
    m_textureTest.init(specification);
}

void Scene::shutdown()
{
}

void Scene::onUpdate(float timeStep)
{
    Renderer::setClearColour(glm::vec4(0.5f, 0.2f, 0.5f, 1.0f));

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

    //TODO: Test texturing
    glEnable(GL_TEXTURE_2D);
    m_textureTest.bind();
    glDisable(GL_LIGHTING);

    glBegin(GL_QUADS);

    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.0f,  1.0f,  0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.0f,  0.0f,  0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f,  0.0f,  0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  0.5f);

    glEnd();

    glBegin(GL_TRIANGLES);

    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 0.5f,  0.5f,  0.5f);

    glEnd();

    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);


    // Render a cube
    Renderer::drawCube(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)), m_materialLibrary.at("WHITE_SHINY"));

    //TODO: temp
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
        std::cout << err << std::endl;
}

void Scene::onUIRender()
{
    ImGui::Begin("Hello World!");
    ImGui::Text("Hello World!");

    ImGui::Image(reinterpret_cast<void*>(static_cast<uint64_t>((m_textureTest.getRendererID()))), { static_cast<float>(m_textureTest.getWidth()), static_cast<float>(m_textureTest.getHeight()) }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

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
    glm::vec4 lightPosition(0.0f, 5.0f, 3.0f, 1.0f);

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
}
