#pragma once

#include <glm/glm.hpp>

class Renderer
{
public:

	static void init();

	static void clear();
	static void setClearColour(const glm::vec4& colour);

	static void setViewport(uint32_t width, uint32_t height);
};