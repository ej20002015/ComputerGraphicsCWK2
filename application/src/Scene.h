#pragma once

#include <unordered_map>

#include "PerspectiveCamera.h"
#include "Material.h"
#include "Texture.h"

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
	void setUpMaterialLibrary();
	void setUpTextures();
	void drawForest();
	void drawTree(const glm::mat4& transform);

private:

	PerspectiveCamera m_camera;
	std::unordered_map<std::string, Material> m_materialLibrary;
	Texture m_textureTest;
	Texture m_grassTexture;
	Texture m_barkTexture;
	Texture m_treeTrunkCrossSectionTexture;
	Texture m_treeCanopyTexture;
};