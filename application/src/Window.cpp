#include "Window.h"

#include <glad/glad.h>

#include "Log.h"

static void glfwErrorCallback(int32_t errorCode, const char* message)
{
    Log::fatal("GLFW ERROR: " + std::string(message));
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
    
    // This data will be sent by glfw when any glfw callback function is called and can be modified
    glfwSetWindowUserPointer(m_glfwWindow, &m_windowCallbacks);

    // Execute my specified callback functions when glfw detects certain window events

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

Window::~Window()
{
    glfwDestroyWindow(m_glfwWindow);
}

void Window::init()
{
    // Initialiase glfw to be able to start making windows
    if (!glfwInit())
        Log::fatal("Cannot initialise GLFW");

    // Set GLFW error callback function
    glfwSetErrorCallback(glfwErrorCallback);
}

void Window::shutdown()
{
    // Shutdown glfw
    glfwTerminate();
}

void Window::onUpdate()
{
    // Swaps the framebuffers to display the next renderered frame on the window
    glfwSwapBuffers(m_glfwWindow);

    // Process events (will call my callback functions if necessary)
    glfwPollEvents();
}

void Window::setWindowCursorDisabled(bool disabled)
{
    if (disabled)
    {
        // Cursor set to behave like it is working within an infinite window and cursor is set to invisible
        // Similar to how a mouse behaves in an fps PC game
        glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_cursorDisabled = true;
    }
    else
    {
        // Cursor set to behave as normal
        glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_cursorDisabled = false;
    }
}

