#include "Scene.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include "Renderer.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::onUpdate(double timestep)
{
    Renderer::setClearColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5, -0.5, 1);
    glVertex3f(0.5, -0.5, 1);
    glVertex3f(0.0, 0.5, 1);
    glEnd();
}

void Scene::onUIRender()
{
    ImGui::Begin("Hello World!");
    ImGui::Text("Hello World!");
    ImGui::End();
}
