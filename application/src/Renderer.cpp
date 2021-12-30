#include "Renderer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "Log.h"

uint32_t Renderer::s_openGLMajorVersion = 0, Renderer::s_openGLMinorVersion = 0;

static void OpenGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		Log::fatal("OPENGL ERROR: " + std::string(message));
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		Log::message("OPENGL WARNING: " + std::string(message));
		break;
    default:
        // Don't care about any lower level messages
        break;
	}
}

static const glm::vec3 cubeVertexPositionsFrontFace[6] =
{
    { -0.5f,  0.5f,  0.5f },
    { -0.5f, -0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },
    { -0.5f, -0.5f,  0.5f },
    {  0.5f, -0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f }
};

static const glm::vec3 cubeVertexPositionsBackFace[6] =
{
    {  0.5f,  0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    { -0.5f,  0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    { -0.5f, -0.5f, -0.5f },
    { -0.5f,  0.5f, -0.5f }
};

static const glm::vec3 cubeVertexPositionsRightFace[6] =
{
    {  0.5f,  0.5f,  0.5f },
    {  0.5f, -0.5f,  0.5f },
    {  0.5f,  0.5f, -0.5f },
    {  0.5f, -0.5f,  0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f,  0.5f, -0.5f }
};

static const glm::vec3 cubeVertexPositionsLeftFace[6] =
{
    { -0.5f,  0.5f, -0.5f },
    { -0.5f, -0.5f, -0.5f },
    { -0.5f,  0.5f,  0.5f },
    { -0.5f, -0.5f, -0.5f },
    { -0.5f, -0.5f,  0.5f },
    { -0.5f,  0.5f,  0.5f }
};

static const glm::vec3 cubeVertexPositionsTopFace[6] =
{
    { -0.5f,  0.5f, -0.5f },
    { -0.5f,  0.5f,  0.5f },
    {  0.5f,  0.5f, -0.5f },
    { -0.5f,  0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },
    {  0.5f,  0.5f, -0.5f }
};

static const glm::vec3 cubeVertexPositionsBottomFace[6] =
{
    { -0.5f, -0.5f,  0.5f },
    { -0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f,  0.5f },
    { -0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f,  0.5f }
};

static const glm::vec2 cubeVertexTexCoords[6] =
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

    // Restore the state of the MODELVIEW matrix
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

    // Only enable and apply texture if one is specified for the face
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

    glNormal3f(1.0f, 0.0f, 0.0f);
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

    glNormal3f(-1.0f, 0.0f, 0.0f);
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

    glNormal3f(0.0f, -1.0f, 0.0f);
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

    // Restore the state of the MODELVIEW matrix
    glPopMatrix();
}

void Renderer::drawCylinder(const glm::mat4& transform, const Material& material, uint32_t LOD)
{
    loadMaterial(material);

    // Apply the transform

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glMultMatrixf(glm::value_ptr(transform));

    float distanceBetweenCirclePoints = 360.0f / static_cast<float>(LOD);

    constexpr glm::vec3 frontCircleCenter = { 0.0f, 0.0f, 0.5f };

    glBegin(GL_TRIANGLES);

    for (float theta = 0.0f; theta < 360.0f; theta += distanceBetweenCirclePoints)
    {
        // Create the cylinder circles using a triangle fan

        glm::vec3 pointOnCircle1 = { 0.5f * glm::sin(glm::radians(theta)) + frontCircleCenter.x, 0.5f * glm::cos(glm::radians(theta)) + frontCircleCenter.y, frontCircleCenter.z };
        glm::vec3 pointOnCircle2 = { 0.5f * glm::sin(glm::radians(theta + distanceBetweenCirclePoints)) + frontCircleCenter.x, 0.5f * glm::cos(glm::radians(theta + distanceBetweenCirclePoints)) + frontCircleCenter.y, frontCircleCenter.z };

        // Draw triangle on the front face

        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(frontCircleCenter.x, frontCircleCenter.y, frontCircleCenter.z);
        glVertex3f(pointOnCircle2.x, pointOnCircle2.y, pointOnCircle2.z);
        glVertex3f(pointOnCircle1.x, pointOnCircle1.y, pointOnCircle1.z);

        // Draw quad for the side of the cylinder

        // Specify different normals for the left and right side of the quad strip to give the appearence of it being rounded
        glm::vec3 leftNormal = glm::normalize(pointOnCircle1 - frontCircleCenter);
        glm::vec3 rightNormal = glm::normalize(pointOnCircle2 - frontCircleCenter);

        glNormal3f(leftNormal.x, leftNormal.y, leftNormal.z);
        glVertex3f(pointOnCircle1.x, pointOnCircle1.y, pointOnCircle1.z - 1.0f);
        glVertex3f(pointOnCircle1.x, pointOnCircle1.y, pointOnCircle1.z);
        glNormal3f(rightNormal.x, rightNormal.y, rightNormal.z);
        glVertex3f(pointOnCircle2.x, pointOnCircle2.y, pointOnCircle2.z - 1.0f);
        glNormal3f(leftNormal.x, leftNormal.y, leftNormal.z);
        glVertex3f(pointOnCircle1.x, pointOnCircle1.y, pointOnCircle1.z);
        glNormal3f(rightNormal.x, rightNormal.y, rightNormal.z);
        glVertex3f(pointOnCircle2.x, pointOnCircle2.y, pointOnCircle2.z);
        glVertex3f(pointOnCircle2.x, pointOnCircle2.y, pointOnCircle2.z - 1.0f);

        // Draw triangle on the back face

        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(frontCircleCenter.x, frontCircleCenter.y, frontCircleCenter.z - 1.0f);
        glVertex3f(pointOnCircle1.x, pointOnCircle1.y, pointOnCircle1.z - 1.0f);
        glVertex3f(pointOnCircle2.x, pointOnCircle2.y, pointOnCircle2.z - 1.0f);
    }

    glEnd();

    // Restore the state of the MODELVIEW matrix
    glPopMatrix();
}

void Renderer::drawCylinder(const glm::mat4& transform, const Material& material, const CylinderTextureSpecification& textureSpecification, float tilingFactor, uint32_t LOD)
{
    loadMaterial(material);

    // Apply the transform

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glMultMatrixf(glm::value_ptr(transform));

    float distanceBetweenCirclePoints = 360.0f / static_cast<float>(LOD);

    constexpr glm::vec3 frontCircleCenter = { 0.0f, 0.0f, 0.5f };

    // Geometry for the 3 faces is split into different loops since that will be faster than calling glBegin and glEnd within a combined loop on each iteration

    // Front face

    if (textureSpecification.frontFace)
    {
        glEnable(GL_TEXTURE_2D);
        textureSpecification.frontFace->bind();
    }

    glBegin(GL_TRIANGLES);

    for (float theta = 0.0f; theta < 360.0f; theta += distanceBetweenCirclePoints)
    {
        // Create the cylinder circles using a triangle fan

        glm::vec3 pointOnCircle1 = { 0.5f * glm::sin(glm::radians(theta)) + frontCircleCenter.x, 0.5f * glm::cos(glm::radians(theta)) + frontCircleCenter.y, frontCircleCenter.z };
        glm::vec3 pointOnCircle2 = { 0.5f * glm::sin(glm::radians(theta + distanceBetweenCirclePoints)) + frontCircleCenter.x, 0.5f * glm::cos(glm::radians(theta + distanceBetweenCirclePoints)) + frontCircleCenter.y, frontCircleCenter.z };

        // Draw triangle on the front face

        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.5f * tilingFactor, 0.5f * tilingFactor);
        glVertex3f(frontCircleCenter.x, frontCircleCenter.y, frontCircleCenter.z);
        glTexCoord2f((pointOnCircle2.x + 0.5f) * tilingFactor, (pointOnCircle2.y + 0.5f) * tilingFactor);
        glVertex3f(pointOnCircle2.x, pointOnCircle2.y, pointOnCircle2.z);
        glTexCoord2f((pointOnCircle1.x + 0.5f) * tilingFactor, (pointOnCircle1.y + 0.5f) * tilingFactor);
        glVertex3f(pointOnCircle1.x, pointOnCircle1.y, pointOnCircle1.z);
    }

    glEnd();

    if (textureSpecification.frontFace)
        glDisable(GL_TEXTURE_2D);

    // Curved face

    if (textureSpecification.curvedFace)
    {
        glEnable(GL_TEXTURE_2D);
        textureSpecification.curvedFace->bind();
    }

    glBegin(GL_TRIANGLES);

    for (float theta = 0.0f; theta < 360.0f; theta += distanceBetweenCirclePoints)
    {
        glm::vec3 pointOnCircle1 = { 0.5f * glm::sin(glm::radians(theta)) + frontCircleCenter.x, 0.5f * glm::cos(glm::radians(theta)) + frontCircleCenter.y, frontCircleCenter.z };
        glm::vec3 pointOnCircle2 = { 0.5f * glm::sin(glm::radians(theta + distanceBetweenCirclePoints)) + frontCircleCenter.x, 0.5f * glm::cos(glm::radians(theta + distanceBetweenCirclePoints)) + frontCircleCenter.y, frontCircleCenter.z };

        // Draw quad for the side of the cylinder

        // Specify different normals for the left and right side of the quad strip to give the appearence of it being rounded
        glm::vec3 leftNormal = glm::normalize(pointOnCircle1 - frontCircleCenter);
        glm::vec3 rightNormal = glm::normalize(pointOnCircle2 - frontCircleCenter);

        float leftTexCoord = glm::mix(0.0f, 1.0f, theta / 360.0f) * tilingFactor;
        float rightTexCoord = glm::mix(0.0f, 1.0f, (theta + distanceBetweenCirclePoints) / 360.0f) * tilingFactor;

        glNormal3f(leftNormal.x, leftNormal.y, leftNormal.z);
        glTexCoord2f(leftTexCoord, 1.0f * tilingFactor);
        glVertex3f(pointOnCircle1.x, pointOnCircle1.y, pointOnCircle1.z - 1.0f);
        glTexCoord2f(leftTexCoord, 0.0f);
        glVertex3f(pointOnCircle1.x, pointOnCircle1.y, pointOnCircle1.z);
        glNormal3f(rightNormal.x, rightNormal.y, rightNormal.z);
        glTexCoord2f(rightTexCoord, 1.0f * tilingFactor);
        glVertex3f(pointOnCircle2.x, pointOnCircle2.y, pointOnCircle2.z - 1.0f);
        glNormal3f(leftNormal.x, leftNormal.y, leftNormal.z);
        glTexCoord2f(leftTexCoord, 0.0f);
        glVertex3f(pointOnCircle1.x, pointOnCircle1.y, pointOnCircle1.z);
        glNormal3f(rightNormal.x, rightNormal.y, rightNormal.z);
        glTexCoord2f(rightTexCoord, 0.0f);
        glVertex3f(pointOnCircle2.x, pointOnCircle2.y, pointOnCircle2.z);
        glTexCoord2f(rightTexCoord, 1.0f * tilingFactor);
        glVertex3f(pointOnCircle2.x, pointOnCircle2.y, pointOnCircle2.z - 1.0f);
    }

    glEnd();

    if (textureSpecification.curvedFace)
        glDisable(GL_TEXTURE_2D);

    // Back face

    if (textureSpecification.backFace)
    {
        glEnable(GL_TEXTURE_2D);
        textureSpecification.backFace->bind();
    }

    glBegin(GL_TRIANGLES);

    for (float theta = 0.0f; theta < 360.0f; theta += distanceBetweenCirclePoints)
    {
        glm::vec3 pointOnCircle1 = { 0.5f * glm::sin(glm::radians(theta)) + frontCircleCenter.x, 0.5f * glm::cos(glm::radians(theta)) + frontCircleCenter.y, frontCircleCenter.z };
        glm::vec3 pointOnCircle2 = { 0.5f * glm::sin(glm::radians(theta + distanceBetweenCirclePoints)) + frontCircleCenter.x, 0.5f * glm::cos(glm::radians(theta + distanceBetweenCirclePoints)) + frontCircleCenter.y, frontCircleCenter.z };

        // Draw triangle on the back face

        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0.5f * tilingFactor, 0.5f * tilingFactor);
        glVertex3f(frontCircleCenter.x, frontCircleCenter.y, frontCircleCenter.z - 1.0f);
        glTexCoord2f((pointOnCircle1.x + 0.5f) * tilingFactor, (pointOnCircle1.y + 0.5f) * tilingFactor);
        glVertex3f(pointOnCircle1.x, pointOnCircle1.y, pointOnCircle1.z - 1.0f);
        glTexCoord2f((pointOnCircle2.x + 0.5f) * tilingFactor, (pointOnCircle2.y + 0.5f) * tilingFactor);
        glVertex3f(pointOnCircle2.x, pointOnCircle2.y, pointOnCircle2.z - 1.0f);
    }

    glEnd();

    if (textureSpecification.backFace)
        glDisable(GL_TEXTURE_2D);

    // Restore the state of the MODELVIEW matrix
    glPopMatrix();
}

void Renderer::drawOctahedron(const glm::mat4& transform, const Material& material)
{
    loadMaterial(material);

    // Apply the transform

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glMultMatrixf(glm::value_ptr(transform));

    // Construct a unit octahedron centered on the origin

    glBegin(GL_TRIANGLES);

    // Top 4 triangles

    glNormal3f(0.0f, 1.0f, 1.0f);
    glVertex3f( 0.0f,  0.5f,  0.0f);
    glVertex3f(-0.5f,  0.0f,  0.5f);
    glVertex3f( 0.5f,  0.0f,  0.5f);

    glNormal3f(0.0f, 1.0f, -1.0f);
    glVertex3f( 0.0f,  0.5f,  0.0f);
    glVertex3f( 0.5f,  0.0f, -0.5f);
    glVertex3f(-0.5f,  0.0f, -0.5f);

    glNormal3f(1.0f, 1.0f, 0.0f);
    glVertex3f( 0.0f,  0.5f,  0.0f);
    glVertex3f( 0.5f,  0.0f,  0.5f);
    glVertex3f( 0.5f,  0.0f, -0.5f);

    glNormal3f(-1.0f, 1.0f, 0.0f);
    glVertex3f( 0.0f,  0.5f,  0.0f);
    glVertex3f(-0.5f,  0.0f, -0.5f);
    glVertex3f(-0.5f,  0.0f,  0.5f);

    // Bottom 4 triangles

    glNormal3f(0.0f, -1.0f, 1.0f);
    glVertex3f( 0.0f, -0.5f,  0.0f);
    glVertex3f( 0.5f,  0.0f,  0.5f);
    glVertex3f(-0.5f,  0.0f,  0.5f);

    glNormal3f(0.0f, -1.0f, -1.0f);
    glVertex3f( 0.0f, -0.5f,  0.0f);
    glVertex3f(-0.5f,  0.0f, -0.5f);
    glVertex3f( 0.5f,  0.0f, -0.5f);

    glNormal3f(1.0f, -1.0f, 0.0f);
    glVertex3f( 0.0f, -0.5f,  0.0f);
    glVertex3f( 0.5f,  0.0f, -0.5f);
    glVertex3f( 0.5f,  0.0f,  0.5f);

    glNormal3f(-1.0f, -1.0f, 0.0f);
    glVertex3f( 0.0f, -0.5f,  0.0f);
    glVertex3f(-0.5f,  0.0f,  0.5f);
    glVertex3f(-0.5f,  0.0f, -0.5f);

    glEnd();

    // Restore the state of the MODELVIEW matrix
    glPopMatrix();
}

void Renderer::init()
{
	// Get OpenGL version so it can be used later to determine which features are available

	const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	Log::message("OpenGL version: " + std::string(version));
	char majorVersionChar = *version;
    char minorVersionChar = *(version + 2);
	s_openGLMajorVersion = majorVersionChar - '0';
    s_openGLMinorVersion = minorVersionChar - '0';

    // Only enable modern error handling using the callback if supported
	if (s_openGLMajorVersion > 3)
	{
		glDebugMessageCallback(OpenGLErrorCallback, nullptr);
    	glEnable(GL_DEBUG_OUTPUT);
    	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

    // Normalise the normals so I don't have to worry about doing that myself
	glEnable(GL_NORMALIZE);

	glEnable(GL_DEPTH_TEST);

    // Use smooth Gouraud shading since that looks better than flat
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
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
