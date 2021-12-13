#include "Scene.h"

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
}

void Scene::shutdown()
{ 
}

void Scene::onUpdate(float timeStep)
{
    Renderer::setClearColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    // If the camera is being used then disable the cursor
    if (m_camera.getCameraActive() && !Application::getWindow().getWindowCursorDisabled())
        Application::getWindow().setWindowCursorDisabled(true);
    else if (!m_camera.getCameraActive() && Application::getWindow().getWindowCursorDisabled())
        Application::getWindow().setWindowCursorDisabled(false);

    // Update the camera
    m_camera.onUpdate(timeStep);

    // Render a cube

    glBegin(GL_TRIANGLES);
    
    glColor4f(0.9f, 0.9f, 0.3f, 1.0f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);

    glColor4f(0.9f, 0.9f, 0.3f, 1.0f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);

    glColor4f(0.4f, 0.3f, 0.8f, 1.0f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);

    glColor4f(0.4f, 0.3f, 0.8f, 1.0f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);

    glColor4f(0.2f, 0.5f, 0.6f, 1.0f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);

    glColor4f(0.2f, 0.5f, 0.6f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);

    glEnd();


    // Set view and projection matrices
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(m_camera.getViewMatrix()));

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(m_camera.getProjectionMatrix()));
}

void Scene::onUIRender()
{
    ImGui::Begin("Hello World!");
    ImGui::Text("Hello World!");
    ImGui::End();
}

void Scene::onWindowResize(uint32_t width, uint32_t height)
{
    m_camera.onWindowResize(width, height);
}
