#pragma once

#include <memory>
#include <string>
#include <functional>

#include <glfw3.h>

class Window
{
public:

	struct WindowProperties
	{
		// Need to define a default constructor for this struct because of a known GCC bug
		// related to inline default initialisation of nested class member variables
		WindowProperties()
			: title("application"), width(1280), height(720) {}

		std::string title = "Application";
		uint32_t width = 1280;
		uint32_t height = 720;
	};

public:

	Window(const WindowProperties& windowProperties = WindowProperties());
	~Window();

	static void init();
	static void shutdown();

	void onUpdate();

	void setWindowCloseCallback(const std::function<void()>& callback) { m_windowCallbacks.windowCloseCallback = callback; }
	void setWindowResizeCallback(const std::function<void(uint32_t, uint32_t)>& callback) { m_windowCallbacks.windowResizeCallback = callback; }

	GLFWwindow* getGlfwWindow() { return m_glfwWindow; }
	WindowProperties getWindowProperties() { return m_windowProperties; }

private:

	struct WindowCallbacks
	{
		std::function<void()> windowCloseCallback;
		std::function<void(uint32_t, uint32_t)> windowResizeCallback;
	};

private:

	WindowProperties m_windowProperties;
	GLFWwindow* m_glfwWindow;
	WindowCallbacks m_windowCallbacks;
};