#include "Renderer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

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

void Renderer::drawCube(const glm::mat4& transform, const Material& material)
{
	loadMaterial(material);

	// Apply the transform
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glMultMatrixf(glm::value_ptr(transform));

	glBegin(GL_TRIANGLES);
    
	// Front and back faces

    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);

    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);

	// Right and left faces

    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);

    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);

	// Top and Bottom faces

    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);

    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);

    glEnd();

	glPopMatrix();
}

void Renderer::init()
{
	glDebugMessageCallback(OpenGLErrorCallback, nullptr);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
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

void Renderer::loadMaterial(const Material& material)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT,    glm::value_ptr(material.ambient));
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    glm::value_ptr(material.diffuse));
    glMaterialfv(GL_FRONT, GL_SPECULAR,   glm::value_ptr(material.specular));
    glMaterialf(GL_FRONT, GL_SHININESS,   material.shininess);
}
