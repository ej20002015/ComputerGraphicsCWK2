#include "Input.h"

#include "Application.h"

GLFWwindow* Input::s_glfwWindow = nullptr;

void Input::init()
{
    s_glfwWindow = Application::getWindow().getGlfwWindow();
}

bool Input::isKeyPressed(KeyCode keyCode)
{
    return glfwGetKey(s_glfwWindow, static_cast<int32_t>(keyCode)) == GLFW_PRESS;
}

bool Input::isMouseButtonPressed(MouseButtonCode mouseButtonCode)
{
    return glfwGetMouseButton(s_glfwWindow, static_cast<int32_t>(mouseButtonCode)) == GLFW_PRESS;
}

glm::vec2 Input::getMousePosition()
{
    double x, y;
    glfwGetCursorPos(s_glfwWindow, &x, &y);
    return { static_cast<float>(x), static_cast<float>(y) };
}