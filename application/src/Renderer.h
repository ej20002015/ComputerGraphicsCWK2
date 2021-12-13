#pragma once

#include <glm/glm.hpp>

#include "Material.h"

class Renderer
{
public:

	static void init();

	// Draw geometry

	static void drawCube(const glm::mat4& transform, const Material& material);

	// Utility methods

	static void clear();
	static void setClearColour(const glm::vec4& colour);

	static void setViewport(uint32_t width, uint32_t height);

private:

	static void loadMaterial(const Material& material);
};