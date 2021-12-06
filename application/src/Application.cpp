#include "Application.h"

#include "glad/glad.h"

//temp

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

Application* Application::s_instance = nullptr;

Application* Application::Create(const std::string& title)
{
    if (s_instance)
        return s_instance;

    s_instance = new Application(title);
    return s_instance;
}

Application::~Application()
{
    if (m_window)
        delete m_window;

    //Temp
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    //End

    Window::shutdown();
}

void Application::run()
{
    while (m_running)
    {

        //Temp
        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //End

        glClear(GL_COLOR_BUFFER_BIT);

        //TODO: Temp - remove
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        glVertex3f(-0.5, -0.5, 1);
        glVertex3f(0.5, -0.5, 1);
        glVertex3f(0.0, 0.5, 1);
        glEnd();

        ImGui::Begin("Hello World!");
        ImGui::Text("Hello World!");
        ImGui::End();

        ImGui::ShowDemoWindow();

        //Temp
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(m_window->getWindowProperties().width), static_cast<float>(m_window->getWindowProperties().height));

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
        //End
        

        //Game loop
        m_window->onUpdate();
    }
}

void Application::onWindowClose()
{
    m_running = false;
}

void Application::onWindowResize(uint32_t width, uint32_t height)
{
    //Temp
    glViewport(0, 0, width, height);
}

Application::Application(const std::string& title)
    : m_running(true)
{
    Window::init();

    // Create window for the application

    Window::WindowProperties windowProperties;
    windowProperties.title = title;
    m_window = new Window(windowProperties);

    // Register callbacks

    m_window->setWindowCloseCallback([this]() -> void { this->onWindowClose(); });
    m_window->setWindowResizeCallback([this](uint32_t width, uint32_t height) -> void { this->onWindowResize(width, height); });


    // Temp ImGui

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window->getGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
}
