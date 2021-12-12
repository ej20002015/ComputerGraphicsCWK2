#pragma once

#include "Window.h"
#include "Scene.h"

class Application
{
public:

	static Application* Create(const std::string& title = "Application");
	~Application();

	void run();
	void onWindowClose();
	void onWindowResize(uint32_t width, uint32_t height);

private:

	Application(const std::string& title);

private:

	static Application* s_instance;
	bool m_running;
	Window* m_window;
	double m_timeAtLastFrame;
	Scene m_scene;
};