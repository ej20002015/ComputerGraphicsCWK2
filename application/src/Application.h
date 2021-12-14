#pragma once

#include "Window.h"
#include "Scene.h"

class Application
{
public:

	static Window& getWindow();
	Application(const std::string& title = "Application");
	~Application();

	void run();
	void onWindowClose();
	void onWindowResize(uint32_t width, uint32_t height);

private:

	static Application* s_instance;
	bool m_running = true;
	Window* m_window;
	float m_timeAtLastFrame = 0.0f;
	Scene m_scene;
};