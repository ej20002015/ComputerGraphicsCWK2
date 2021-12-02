#include "Application.h"

#include "glad/glad.h"

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

    Window::shutdown();
}

void Application::run()
{
    while (m_running)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        //TODO: Temp - remove
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        glVertex3f(-0.5, -0.5, 1);
        glVertex3f(0.5, -0.5, 1);
        glVertex3f(0.0, 0.5, 1);
        glEnd();

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
}
