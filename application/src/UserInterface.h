#pragma once

#include <glfw3.h>

class UserInterface
{
public:

    static void init();
    static void shutdown();

    static void startFrame();
    static void endFrame();
};