#pragma once

#include "PerspectiveCamera.h"

class Scene
{
public:

	void init(uint32_t windowWidth, uint32_t windowHeight);
	void shutdown();

	Scene() = default;
	~Scene() = default;

	void onUpdate(double timestep);

	void onUIRender();

	void onWindowResize(uint32_t width, uint32_t height);

private:

	PerspectiveCamera m_camera;
};