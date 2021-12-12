#pragma once

#include <glfw3.h>

class UserInterface
{
public:

    static void init(GLFWwindow* glfwWindow);
    static void shutdown();

    static void startFrame();
    static void endFrame(uint32_t width, uint32_t height);
};