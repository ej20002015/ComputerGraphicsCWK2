#pragma once

class Scene
{
public:

	Scene();
	~Scene();

	void onUpdate(double timestep);

	void onUIRender();
};