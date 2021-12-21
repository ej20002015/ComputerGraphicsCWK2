#pragma once

#include <glm/glm.hpp>

#include "Material.h"
#include "Texture.h"

class Renderer
{
public:

	struct CubeTextureSpecification
	{
		CubeTextureSpecification() = default;
		CubeTextureSpecification(const Texture* texture)
			: frontFace(texture), backFace(texture), rightFace(texture), leftFace(texture), topFace(texture), bottomFace(texture) {}
			
		const Texture* frontFace = nullptr;
		const Texture* backFace = nullptr;
		const Texture* rightFace = nullptr;
		const Texture* leftFace = nullptr;
		const Texture* topFace = nullptr;
		const Texture* bottomFace = nullptr;
	};

	struct CylinderTextureSpecification
	{
		CylinderTextureSpecification() = default;
		CylinderTextureSpecification(const Texture * texture)
			: frontFace(texture), curvedFace(texture), backFace(texture) {}

		const Texture* frontFace = nullptr;
		const Texture* curvedFace = nullptr;
		const Texture* backFace = nullptr;
	};

public:

	static void init();

	// Drawing methods

	static void drawCube(const glm::mat4& transform, const Material& material);
	static void drawCube(const glm::mat4& transform, const Material& material, const CubeTextureSpecification& textureSpecification, float tilingFactor = 1.0f);

	static void drawCylinder(const glm::mat4& transform, const Material& material, uint32_t LOD = 32);
	static void drawCylinder(const glm::mat4& transform, const Material& material, const CylinderTextureSpecification& textureSpecification, float tilingFactor = 1.0f, uint32_t LOD = 32);

	static void drawOctahedron(const glm::mat4& transform, const Material& material);

	static void drawCircle(const glm::mat4& transform, const Material& material, uint32_t LOD = 32);

	// Utility methods

	static void clear();
	static void setClearColour(const glm::vec4& colour);

	static void setViewport(uint32_t width, uint32_t height);

	static uint32_t getOpenGLMajorVersion() { return s_openGLMajorVersion; }
	static uint32_t getOpenGLMinorVersion() { return s_openGLMinorVersion; }

private:

	static void loadMaterial(const Material& material);

private:

	static uint32_t s_openGLMajorVersion, s_openGLMinorVersion;
};