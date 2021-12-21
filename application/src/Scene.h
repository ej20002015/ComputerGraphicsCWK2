#pragma once

#include <array>

#include "PerspectiveCamera.h"
#include "Texture.h"
#include "Light.h"

class Scene
{
public:

	void init();
	void shutdown();

	Scene() = default;
	~Scene() = default;

	void onUpdate(float timeStep);

	void onUIRender();

	void onWindowResize(uint32_t width, uint32_t height);

private:

	void setUpLights();
	void setUpTextures();
	void drawForest();
	void drawTable(const glm::mat4& transform);
	void drawTree(const glm::mat4& transform);

private:

	PerspectiveCamera m_camera;

	std::array<Light, 1> m_lights;
	bool m_showLightPositions = false;

	//TODO: REMOVE
	Texture m_textureTest;

	Texture m_grassTexture;
	Texture m_barkTexture;
	Texture m_treeTrunkCrossSectionTexture;
	Texture m_treeCanopyTexture;
	Texture m_tableTopTexture;
	Texture m_tableLegTexture;
	Texture m_mapTexture;

	float m_spinningTopRotationAngle = 0.0f;
	float m_spinningTopRotationSpeed = 1.0f;
};