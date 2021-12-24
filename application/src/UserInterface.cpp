#include "UserInterface.h"

#include <glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Application.h"
#include "Renderer.h"

ImFont* UserInterface::s_boldFont = nullptr;

/*
* Boilerplate ImGui code taken from the imgui/examples/example_glfw_opengl3 directory
*/

void UserInterface::init()
{  
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    GLFWwindow* glfwWindow = Application::getWindow().getGlfwWindow();
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);

    switch (Renderer::getOpenGLMajorVersion())
    {
        case 4:
            ImGui_ImplOpenGL3_Init("#version 460");
            break;
        default:
            ImGui_ImplOpenGL3_Init("#version 130");
            break;
    }

    // Swap out the default font for something that looks better
    ImFont* font = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf", 16.0f);
    s_boldFont = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Bold.ttf", 16.0f);
    if (font)
        io.FontDefault = font;
}

void UserInterface::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UserInterface::startFrame()
{
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UserInterface::endFrame()
{
    ImGuiIO& io = ImGui::GetIO();
    uint32_t windowWidth = Application::getWindow().getWindowProperties().width;
    uint32_t windowHeight = Application::getWindow().getWindowProperties().height;
    io.DisplaySize = ImVec2(static_cast<float>(windowWidth), static_cast<float>(windowHeight));

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void UserInterface::pushBoldFont()
{
    if (s_boldFont)
        ImGui::PushFont(s_boldFont);
}

void UserInterface::popBoldFont()
{
    ImGui::PopFont();
}
