#include "Scene.h"

//TODO: Temp
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "Renderer.h"
#include "Application.h"

void Scene::init()
{
    uint32_t windowWidth = Application::getWindow().getWindowProperties().width;
    uint32_t windowHeight = Application::getWindow().getWindowProperties().height;
    m_camera.init(windowWidth, windowHeight);

    setUpLights();
    setUpMaterialLibrary();
    setUpTextures();
}

void Scene::shutdown()
{
}

void Scene::onUpdate(float timeStep)
{
    Renderer::setClearColour(glm::vec4(0.396f, 0.761f, 0.961f, 1.0f));

    // If the camera is being used then disable the cursor
    if (m_camera.getCameraActive() && !Application::getWindow().getWindowCursorDisabled())
        Application::getWindow().setWindowCursorDisabled(true);
    else if (!m_camera.getCameraActive() && Application::getWindow().getWindowCursorDisabled())
        Application::getWindow().setWindowCursorDisabled(false);

    // Update the camera
    m_camera.onUpdate(timeStep);

    // Set view and projection matrices
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(m_camera.getViewMatrix()));

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(m_camera.getProjectionMatrix()));

    // Render some ground
    Renderer::CubeTextureSpecification textureSpecificationGround;
    textureSpecificationGround.topFace = &m_grassTexture;
    Renderer::drawCube(glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 0.5f, 50.0f)), m_materialLibrary.at("NO_MATERIAL"), textureSpecificationGround, 25.0f);

    Renderer::drawCube(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)), m_materialLibrary.at("GOLD"));

    drawForest();

    // Render a textured cube

    Renderer::CubeTextureSpecification textureSpecificationCube;
    textureSpecificationCube.frontFace = &m_textureTest;
    textureSpecificationCube.backFace = &m_textureTest;
    textureSpecificationCube.rightFace = &m_textureTest;
    textureSpecificationCube.leftFace = &m_textureTest;
    textureSpecificationCube.topFace = &m_textureTest;
    textureSpecificationCube.bottomFace = &m_textureTest;
    Renderer::drawCube(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 0.0f)), m_materialLibrary.at("GOLD"), textureSpecificationCube);
}

void Scene::onUIRender()
{
    ImGui::Begin("Hello World!");
    ImGui::Text("Hello World!");

    static bool smoothShading = true;
    if (ImGui::Checkbox("Smooth", &smoothShading))
    {
        if (smoothShading)
            glShadeModel(GL_SMOOTH);
        else
            glShadeModel(GL_FLAT);
    }

    bool cameraLockYAxisPosition = m_camera.getLockYAxisPosition();
    if (ImGui::Checkbox("Lock Camera Y Axis Position", &cameraLockYAxisPosition))
        m_camera.setLockYAxisPosition(cameraLockYAxisPosition);

    ImGui::End();
}

void Scene::onWindowResize(uint32_t width, uint32_t height)
{
    m_camera.onWindowResize(width, height);
}

void Scene::setUpLights()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glLoadIdentity();

    glEnable(GL_LIGHT0);
    glm::vec4 lightPosition(2.0f, 2.0f, 0.0f, 1.0f);

    glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(lightPosition));
    glPopMatrix();
}

void Scene::setUpMaterialLibrary()
{
    m_materialLibrary["WHITE_SHINY"] =
    {
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        100.0f
    };

    m_materialLibrary["BLACK_PLASTIC"] =
    {
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.001f, 0.001f, 0.001f, 1.0f },
        { 0.5f, 0.5f, 0.5f, 1.0f },
        32.0f
    };

    m_materialLibrary["GOLD"] =
    {
        { 0.24725f, 0.1995f, 0.0745f, 1.0f },
        { 0.75164f, 0.60648f, 0.22648f, 1.0f },
        { 0.628281f, 0.555802f, 0.366065f, 1.0f },
        51.2f
    };

    m_materialLibrary["NO_MATERIAL"] =
    {
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        0.0f
    };
}

void Scene::setUpTextures()
{
    Texture::TextureSpecification testTexturespecification;
    testTexturespecification.wrappingMode = Texture::WrappingMode::REPEAT;
    testTexturespecification.filepath = "assets/textures/markus.jpg";
    m_textureTest.init(testTexturespecification);

    Texture::TextureSpecification grassTextureSpecfication;
    grassTextureSpecfication.wrappingMode = Texture::WrappingMode::REPEAT;
    grassTextureSpecfication.filepath = "assets/textures/grass.jpg";
    m_grassTexture.init(grassTextureSpecfication);

    Texture::TextureSpecification barkTextureSpecfication;
    barkTextureSpecfication.wrappingMode = Texture::WrappingMode::REPEAT;
    barkTextureSpecfication.filepath = "assets/textures/Bark.jpg";
    m_barkTexture.init(barkTextureSpecfication);

    Texture::TextureSpecification treeTrunkCrossSectionTextureSpecification;
    treeTrunkCrossSectionTextureSpecification.wrappingMode = Texture::WrappingMode::REPEAT;
    treeTrunkCrossSectionTextureSpecification.filepath = "assets/textures/treeTrunkCrossSection.jpg";
    m_treeTrunkCrossSectionTexture.init(treeTrunkCrossSectionTextureSpecification);

    Texture::TextureSpecification treeCanopyTextureSpecification;
    treeCanopyTextureSpecification.wrappingMode = Texture::WrappingMode::REPEAT;
    treeCanopyTextureSpecification.filepath = "assets/textures/treeCanopy.jpg";
    m_treeCanopyTexture.init(treeCanopyTextureSpecification);
}

void Scene::drawForest()
{
    static const uint32_t treeCount = 18;
    static const glm::vec3 treePositions[treeCount] =
    {
        {   5.0f, 0.0f,  14.0f },
        {   7.5f, 0.0f,  11.0f },
        {  10.0f, 0.0f,  15.0f },
        {  14.0f, 0.0f,   5.0f },
        {  17.0f, 0.0f,  14.0f },
        {   3.0f, 0.0f, -14.0f },
        {   3.0f, 0.0f, -24.0f },
        {   5.0f, 0.0f,  -6.5f },
        {  13.0f, 0.0f, -12.5f },
        { -17.0f, 0.0f, -18.0f },
        { -13.0f, 0.0f,  -4.0f },
        {  -9.0f, 0.0f, -18.0f },
        {  -6.0f, 0.0f,  -7.0f },
        { -15.0f, 0.0f,   5.0f },
        { -13.5f, 0.0f,  21.0f },
        { -10.0f, 0.0f,   7.0f },
        {  -7.0f, 0.0f,   4.0f },
        {  -4.0f, 0.0f,   4.0f }
    };

    static float treeRotations[treeCount] =
    {
        68,
        27,
        84,
        77,
        79,
        53,
        80,
        13,
        45,
        5,
        3,
        8,
        46,
        31,
        64,
        83,
        58,
        70
    };

    for (uint32_t i = 0; i < treeCount; i++)
    {
        glm::vec3 treePosition = treePositions[i];
        float treeRotation = treeRotations[i];
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), { treePosition.x, treePosition.y, treePosition.z });
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), treeRotation, { 0.0f, 1.0f, 0.0f });
        drawTree(translation * rotation);
    }
}

void Scene::drawTree(const glm::mat4& transform)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glMultMatrixf(glm::value_ptr(transform));

    // Draw the trunk of the tree

    Renderer::CylinderTextureSpecification textureSpecificationTrunk;
    textureSpecificationTrunk.frontFace = &m_treeTrunkCrossSectionTexture;
    textureSpecificationTrunk.curvedFace = &m_barkTexture;
    textureSpecificationTrunk.backFace = &m_treeTrunkCrossSectionTexture;
    glm::mat4 trunkTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, 1.4f, 0.0f });
    glm::mat4 trunkRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
    glm::mat4 trunkScale = glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 3.0f });
    Renderer::drawCylinder(trunkTranslation * trunkRotation * trunkScale, m_materialLibrary.at("NO_MATERIAL"), textureSpecificationTrunk);

    // Draw the tree canopy
    Renderer::CubeTextureSpecification textureSpecificationCanopy;
    textureSpecificationCanopy.frontFace = &m_treeCanopyTexture;
    textureSpecificationCanopy.backFace = &m_treeCanopyTexture;
    textureSpecificationCanopy.rightFace = &m_treeCanopyTexture;
    textureSpecificationCanopy.leftFace = &m_treeCanopyTexture;
    textureSpecificationCanopy.topFace = &m_treeCanopyTexture;
    textureSpecificationCanopy.bottomFace = &m_treeCanopyTexture;
    glm::mat4 canopyTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, 2.5f, 0.0f });
    glm::mat4 canopyScale = glm::scale(glm::mat4(1.0f), { 2.0f, 2.0f, 2.0f });
    Renderer::drawCube(canopyTranslation * canopyScale, m_materialLibrary.at("NO_MATERIAL"), textureSpecificationCanopy);


    glPopMatrix();
}
