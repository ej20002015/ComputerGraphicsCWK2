#include "Renderer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Log.h"

uint32_t Renderer::s_openGLMajorVersion = 0;

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

static glm::vec3 cubeVertexPositionsFrontFace[6] =
{
    { -0.5f,  0.5f,  0.5f },
    { -0.5f, -0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },
    { -0.5f, -0.5f,  0.5f },
    {  0.5f, -0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f }
};

static glm::vec3 cubeVertexPositionsBackFace[6] =
{
    {  0.5f,  0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    { -0.5f,  0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    { -0.5f, -0.5f, -0.5f },
    { -0.5f,  0.5f, -0.5f }
};

static glm::vec3 cubeVertexPositionsRightFace[6] =
{
    {  0.5f,  0.5f,  0.5f },
    {  0.5f, -0.5f,  0.5f },
    {  0.5f,  0.5f, -0.5f },
    {  0.5f, -0.5f,  0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f,  0.5f, -0.5f }
};

static glm::vec3 cubeVertexPositionsLeftFace[6] =
{
    { -0.5f,  0.5f, -0.5f },
    { -0.5f, -0.5f, -0.5f },
    { -0.5f,  0.5f,  0.5f },
    { -0.5f, -0.5f, -0.5f },
    { -0.5f, -0.5f,  0.5f },
    { -0.5f,  0.5f,  0.5f }
};

static glm::vec3 cubeVertexPositionsTopFace[6] =
{
    { -0.5f,  0.5f, -0.5f },
    { -0.5f,  0.5f,  0.5f },
    {  0.5f,  0.5f, -0.5f },
    { -0.5f,  0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },
    {  0.5f,  0.5f, -0.5f }
};

static glm::vec3 cubeVertexPositionsBottomFace[6] =
{
    { -0.5f, -0.5f,  0.5f },
    { -0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f,  0.5f },
    { -0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f,  0.5f }
};

static glm::vec2 cubeVertexTexCoords[6] =
{
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f }
};

void Renderer::drawCube(const glm::mat4& transform, const Material& material)
{
	loadMaterial(material);

	// Apply the transform
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glMultMatrixf(glm::value_ptr(transform));

	glBegin(GL_TRIANGLES);
    
	// Front and back faces

    glNormal3f( 0.0f,  0.0f,  1.0f);
    for (uint32_t i = 0; i < 6; i++)
        glVertex3f(cubeVertexPositionsFrontFace[i].x, cubeVertexPositionsFrontFace[i].y, cubeVertexPositionsFrontFace[i].z);

    glNormal3f( 0.0f,  0.0f, -1.0f);
    for (uint32_t i = 0; i < 6; i++)
        glVertex3f(cubeVertexPositionsBackFace[i].x, cubeVertexPositionsBackFace[i].y, cubeVertexPositionsBackFace[i].z);

	// Right and left faces

    glNormal3f( 1.0f,  0.0f,  0.0f);
    for (uint32_t i = 0; i < 6; i++)
        glVertex3f(cubeVertexPositionsRightFace[i].x, cubeVertexPositionsRightFace[i].y, cubeVertexPositionsRightFace[i].z);

    glNormal3f(-1.0f,  0.0f,  0.0f);
    for (uint32_t i = 0; i < 6; i++)
        glVertex3f(cubeVertexPositionsLeftFace[i].x, cubeVertexPositionsLeftFace[i].y, cubeVertexPositionsLeftFace[i].z);

	// Top and Bottom faces

    glNormal3f( 0.0f,  1.0f,  0.0f);
    for (uint32_t i = 0; i < 6; i++)
        glVertex3f(cubeVertexPositionsTopFace[i].x, cubeVertexPositionsTopFace[i].y, cubeVertexPositionsTopFace[i].z);

    glNormal3f( 0.0f, -1.0f,  0.0f);
    for (uint32_t i = 0; i < 6; i++)
        glVertex3f(cubeVertexPositionsBottomFace[i].x, cubeVertexPositionsBottomFace[i].y, cubeVertexPositionsBottomFace[i].z);

    glEnd();

	glPopMatrix();
}

void Renderer::drawCube(const glm::mat4& transform, const Material& material, const CubeTextureSpecification& textureSpecification, float tilingFactor)
{
    loadMaterial(material);

    // Apply the transform
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glMultMatrixf(glm::value_ptr(transform));

    // Front face

    if (textureSpecification.frontFace)
    {
        glEnable(GL_TEXTURE_2D);
        textureSpecification.frontFace->bind();
    }

    glBegin(GL_TRIANGLES);

    glNormal3f(0.0f, 0.0f, 1.0f);
    for (uint32_t i = 0; i < 6; i++)
    {
        glm::vec2 tiledTexCoords = cubeVertexTexCoords[i] * tilingFactor;
        const glm::vec3& vertexPositions = cubeVertexPositionsFrontFace[i];
        glTexCoord2f(tiledTexCoords.x, tiledTexCoords.y);
        glVertex3f(vertexPositions.x, vertexPositions.y, vertexPositions.z);
    }
    
    glEnd();

    if (textureSpecification.frontFace)
        glDisable(GL_TEXTURE_2D);

    // Back face

    if (textureSpecification.backFace)
    {
        glEnable(GL_TEXTURE_2D);
        textureSpecification.backFace->bind();
    }

    glBegin(GL_TRIANGLES);

    glNormal3f(0.0f, 0.0f, -1.0f);
    for (uint32_t i = 0; i < 6; i++)
    {
        glm::vec2 tiledTexCoords = cubeVertexTexCoords[i] * tilingFactor;
        const glm::vec3& vertexPositions = cubeVertexPositionsBackFace[i];
        glTexCoord2f(tiledTexCoords.x, tiledTexCoords.y);
        glVertex3f(vertexPositions.x, vertexPositions.y, vertexPositions.z);
    }

    glEnd();

    if (textureSpecification.backFace)
        glDisable(GL_TEXTURE_2D);

    // Right face

    if (textureSpecification.rightFace)
    {
        glEnable(GL_TEXTURE_2D);
        textureSpecification.rightFace->bind();
    }

    glBegin(GL_TRIANGLES);

    glNormal3f(0.0f, 1.0f, 0.0f);
    for (uint32_t i = 0; i < 6; i++)
    {
        glm::vec2 tiledTexCoords = cubeVertexTexCoords[i] * tilingFactor;
        const glm::vec3& vertexPositions = cubeVertexPositionsRightFace[i];
        glTexCoord2f(tiledTexCoords.x, tiledTexCoords.y);
        glVertex3f(vertexPositions.x, vertexPositions.y, vertexPositions.z);
    }

    glEnd();

    if (textureSpecification.rightFace)
        glDisable(GL_TEXTURE_2D);

    // Left face

    if (textureSpecification.leftFace)
    {
        glEnable(GL_TEXTURE_2D);
        textureSpecification.leftFace->bind();
    }

    glBegin(GL_TRIANGLES);

    glNormal3f(0.0f, -1.0f, 0.0f);
    for (uint32_t i = 0; i < 6; i++)
    {
        glm::vec2 tiledTexCoords = cubeVertexTexCoords[i] * tilingFactor;
        const glm::vec3& vertexPositions = cubeVertexPositionsLeftFace[i];
        glTexCoord2f(tiledTexCoords.x, tiledTexCoords.y);
        glVertex3f(vertexPositions.x, vertexPositions.y, vertexPositions.z);
    }

    glEnd();

    if (textureSpecification.leftFace)
        glDisable(GL_TEXTURE_2D);

    // Top face

    if (textureSpecification.topFace)
    {
        glEnable(GL_TEXTURE_2D);
        textureSpecification.topFace->bind();
    }

    glBegin(GL_TRIANGLES);

    glNormal3f(0.0f, 1.0f, 0.0f);
    for (uint32_t i = 0; i < 6; i++)
    {
        glm::vec2 tiledTexCoords = cubeVertexTexCoords[i] * tilingFactor;
        const glm::vec3& vertexPositions = cubeVertexPositionsTopFace[i];
        glTexCoord2f(tiledTexCoords.x, tiledTexCoords.y);
        glVertex3f(vertexPositions.x, vertexPositions.y, vertexPositions.z);
    }

    glEnd();

    if (textureSpecification.topFace)
        glDisable(GL_TEXTURE_2D);
    
    // Bottom face

    if (textureSpecification.bottomFace)
    {
        glEnable(GL_TEXTURE_2D);
        textureSpecification.bottomFace->bind();
    }

    glBegin(GL_TRIANGLES);

    glNormal3f(0.0f, 1.0f, 0.0f);
    for (uint32_t i = 0; i < 6; i++)
    {
        glm::vec2 tiledTexCoords = cubeVertexTexCoords[i] * tilingFactor;
        const glm::vec3& vertexPositions = cubeVertexPositionsBottomFace[i];
        glTexCoord2f(tiledTexCoords.x, tiledTexCoords.y);
        glVertex3f(vertexPositions.x, vertexPositions.y, vertexPositions.z);
    }

    glEnd();

    if (textureSpecification.bottomFace)
        glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void Renderer::init()
{
	// Get OpenGL version to determine whether
	const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	Log::message("OpenGL version: " + std::string(version));
	char majorVersionChar = *version;
	s_openGLMajorVersion = majorVersionChar - '0';

	if (s_openGLMajorVersion > 3)
	{
		glDebugMessageCallback(OpenGLErrorCallback, nullptr);
    	glEnable(GL_DEBUG_OUTPUT);
    	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
    
    // Enable backface culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
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
