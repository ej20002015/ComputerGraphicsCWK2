#pragma once

#include <glfw3.h>
#include <glm/glm.hpp>

#include "KeyCode.h"

class Input
{
public:

    static void init();

    static bool isKeyPressed(KeyCode keyCode);
    static bool isMouseButtonPressed(MouseButtonCode mouseButtonCode);
    static glm::vec2 getMousePosition();

private:
    static GLFWwindow* s_glfwWindow;
};