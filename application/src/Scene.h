#pragma once

#include <array>

#include "PerspectiveCamera.h"
#include "Texture.h"
#include "Light.h"
#include "Lumberjack.h"

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
	void drawLumberjacksScene(const glm::mat4& transform, float timeStep);
	void drawTable(const glm::mat4& transform, float timeStep);
	void drawTree(const glm::mat4& transform);

private:

	PerspectiveCamera m_camera;

	std::array<Light, 1> m_lights;
	bool m_showLightPositions = false;

	Texture m_grassTexture;
	Texture m_barkTexture;
	Texture m_treeTrunkCrossSectionTexture;
	Texture m_treeCanopyTexture;
	Texture m_tableTopTexture;
	Texture m_tableLegTexture;
	Texture m_mapTexture;
	Texture m_markusTexture;
	Texture m_marcTexture;

	float m_spinningTopRotationAngle = 0.0f;
	float m_spinningTopRotationSpeed = 300.0f;

	Lumberjack m_lumberjack1;
	Lumberjack m_lumberjack2;
	float m_lumberjacksRotationAngle = 0.0f;
	float m_lumberjacksRotationSpeed = 50.0f;
};