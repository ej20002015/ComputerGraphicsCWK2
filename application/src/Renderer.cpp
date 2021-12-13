#include "Renderer.h"

#include <glad/glad.h>

#include "Log.h"

static void OpenGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		Log::fatal("OPENGL ERROR: " + std::string(message));
		break;
	default:
		Log::message("OPENGL ERROR: " + std::string(message));
		break;
	}
}

void Renderer::init()
{
	//glDebugMessageCallback(OpenGLErrorCallback, nullptr);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
}

void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setClearColour(const glm::vec4& colour)
{
	glClearColor(colour.r, colour.g, colour.b, colour.a);
}

void Renderer::setViewport(uint32_t width, uint32_t height)
{
	glViewport(0, 0, static_cast<int32_t>(width), static_cast<int32_t>(height));
}
