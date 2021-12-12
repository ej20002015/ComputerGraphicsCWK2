#pragma once

class Scene
{
public:

	void init();
	void shutdown();

	Scene() = default;
	~Scene() = default;

	void onUpdate(double timestep);

	void onUIRender();
};