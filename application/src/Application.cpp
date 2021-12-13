#include "Application.h"

#include "UserInterface.h"
#include "Renderer.h"
#include "Input.h"
#include "Log.h"

Application* Application::s_instance = nullptr;

Window& Application::getWindow()
{
    return *(s_instance->m_window);
}

Application::Application(const std::string& title)
{
    if (s_instance)
        Log::fatal("Application instance already exists!");

    s_instance = this;

    Window::init();

    // Create window for the application

    Window::WindowProperties windowProperties;
    windowProperties.title = title;
    m_window = new Window(windowProperties);

    // Register callbacks

    m_window->setWindowCloseCallback([this]() -> void { this->onWindowClose(); });
    m_window->setWindowResizeCallback([this](uint32_t width, uint32_t height) -> void { this->onWindowResize(width, height); });

    // Initialise Input
    Input::init();

    // Initialise the renderer
    Renderer::init();

    // Initialise ImGui
    UserInterface::init();

    // Initialise Scene
    m_scene.init();
}

Application::~Application()
{
    m_scene.shutdown();

    if (m_window)
        UserInterface::shutdown();
        delete m_window;

    Window::shutdown();
}

void Application::run()
{
    while (m_running)
    {
        //Calculate time between frames
        float currentTime = m_window->getCurrentTime();
        float timeStep = currentTime - m_timeAtLastFrame;
        m_timeAtLastFrame = currentTime;

        Renderer::clear();

        m_scene.onUpdate(timeStep);

        UserInterface::startFrame();

        m_scene.onUIRender();

        UserInterface::endFrame();

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
    Renderer::setViewport(width, height);
    m_scene.onWindowResize(width, height);
}
