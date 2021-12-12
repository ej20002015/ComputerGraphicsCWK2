#include "Application.h"

#include "UserInterface.h"
#include "Renderer.h"

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
        UserInterface::shutdown();
        delete m_window;

    Window::shutdown();
}

void Application::run()
{
    while (m_running)
    {
        //Calculate time between frames
        double currentTime = m_window->getCurrentTime();
        double timeStep = currentTime - m_timeAtLastFrame;
        m_timeAtLastFrame = currentTime;

        Renderer::clear();

        m_scene.onUpdate(timeStep);

        UserInterface::startFrame();

        m_scene.onUIRender();

        UserInterface::endFrame(m_window->getWindowProperties().width, m_window->getWindowProperties().height);

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
}

Application::Application(const std::string& title)
    : m_running(true), m_timeAtLastFrame(0.0f)
{
    Window::init();

    // Create window for the application

    Window::WindowProperties windowProperties;
    windowProperties.title = title;
    m_window = new Window(windowProperties);

    // Register callbacks

    m_window->setWindowCloseCallback([this]() -> void { this->onWindowClose(); });
    m_window->setWindowResizeCallback([this](uint32_t width, uint32_t height) -> void { this->onWindowResize(width, height); });

    // Initialise the renderer
    Renderer::init();

    // Initialise ImGui
    UserInterface::init(m_window->getGlfwWindow());
}
