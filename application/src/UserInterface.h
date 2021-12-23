#pragma once

#include <imgui.h>

class UserInterface
{
public:

    static void init();
    static void shutdown();

    static void startFrame();
    static void endFrame();

    // Utility methods

    static void pushBoldFont();
    static void popBoldFont();

private:

    static ImFont* s_boldFont;
};