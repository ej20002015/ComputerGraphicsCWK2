#pragma once

#include <glm/glm.hpp>

#include "Material.h"
#include "Texture.h"

class Renderer
{
public:

	struct CubeTextureSpecification
	{
		const Texture* frontFace = nullptr;
		const Texture* backFace = nullptr;
		const Texture* rightFace = nullptr;
		const Texture* leftFace = nullptr;
		const Texture* topFace = nullptr;
		const Texture* bottomFace = nullptr;
	};

public:

	static void init();

	// Draw geometry

	static void drawCube(const glm::mat4& transform, const Material& material);
	static void drawCube(const glm::mat4& transform, const Material& material, const CubeTextureSpecification& textureSpecification, float tilingFactor = 1.0f);

	// Utility methods

	static void clear();
	static void setClearColour(const glm::vec4& colour);

	static void setViewport(uint32_t width, uint32_t height);

	static uint32_t getOpenGLMajorVersion() { return s_openGLMajorVersion; }

private:

	static void loadMaterial(const Material& material);

private:

	static uint32_t s_openGLMajorVersion;
};