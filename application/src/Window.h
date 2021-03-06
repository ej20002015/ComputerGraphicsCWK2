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
	void setWindowCursorDisabled(bool disabled);

	GLFWwindow* getGlfwWindow() const { return m_glfwWindow; }
	WindowProperties getWindowProperties() const { return m_windowProperties; }
	float getCurrentTime() const { return static_cast<float>(glfwGetTime()); }
	bool getWindowCursorDisabled() const { return m_cursorDisabled; }

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
	bool m_cursorDisabled = false;
};