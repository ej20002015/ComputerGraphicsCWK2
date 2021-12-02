#include "Window.h"

#include <glad/glad.h>

#include "Log.h"

static void glfwErrorCallback(int32_t errorCode, const char* message)
{
    Log::fatal("GLFW ERROR: " + std::string(message));
}

Window::~Window()
{
    glfwDestroyWindow(m_glfwWindow);
}

void Window::init()
{
    //Initialiase glfw to be able to start making windows
    if (!glfwInit())
        Log::fatal("Cannot initialise GLFW");

    //Set GLFW error callback function
    glfwSetErrorCallback(glfwErrorCallback);
}

void Window::shutdown()
{
    //Shutdown glfw
    glfwTerminate();
}

void Window::onUpdate()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(m_glfwWindow);

    /* Poll for and process events */
    glfwPollEvents();
}

Window::Window(const WindowProperties& windowProperties)
    : m_windowProperties(windowProperties), m_glfwWindow(nullptr)
{
    m_glfwWindow = glfwCreateWindow(m_windowProperties.width, m_windowProperties.height, m_windowProperties.title.c_str(), NULL, NULL);
    if (!m_glfwWindow)
    {
        glfwTerminate();
        Log::fatal("Cannot create GLFW window");
    }

    glfwMakeContextCurrent(m_glfwWindow);
    gladLoadGL();


    //Deal with window events
    
    //This data will be sent by glfw when any glfw callback function is called and can be modified
    glfwSetWindowUserPointer(m_glfwWindow, &m_windowCallbacks);

    glfwSetWindowCloseCallback(m_glfwWindow, [](GLFWwindow* window)
    {
        WindowCallbacks& windowCallbacks = *static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window));
        if (windowCallbacks.windowCloseCallback)
            windowCallbacks.windowCloseCallback();
    });

    glfwSetWindowSizeCallback(m_glfwWindow, [](GLFWwindow* window, int width, int height)
    {
        WindowCallbacks& windowCallbacks = *static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window));
        if (windowCallbacks.windowResizeCallback)
            windowCallbacks.windowResizeCallback(width, height);
    });
}

