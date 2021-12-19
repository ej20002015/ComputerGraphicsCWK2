#include "Scene.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "Renderer.h"
#include "Application.h"
#include "Material.h"

void Scene::init()
{
    uint32_t windowWidth = Application::getWindow().getWindowProperties().width;
    uint32_t windowHeight = Application::getWindow().getWindowProperties().height;
    m_camera.init(windowWidth, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    MaterialLibrary::init();
    setUpTextures();
    setUpLights();
}

void Scene::shutdown()
{
}

void Scene::onUpdate(float timeStep)
{
    Renderer::setClearColour(glm::vec4(0.396f, 0.761f, 0.961f, 1.0f));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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

    for (auto light : m_lights)
        light.update();

    // See light location

    m_lights[0].renderLocation();

    // Render some ground
    
    Renderer::CubeTextureSpecification textureSpecificationGround;
    textureSpecificationGround.topFace = &m_grassTexture;
    Renderer::drawCube(glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 0.5f, 50.0f)), MaterialLibrary::getMaterial("FAUNA"), textureSpecificationGround, 25.0f);

    Renderer::drawCube(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)), MaterialLibrary::getMaterial("GOLD"));

    drawForest();

    // Render a textured cube

    Renderer::CubeTextureSpecification textureSpecificationCube(&m_textureTest);
    Renderer::drawCube(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 0.0f)), MaterialLibrary::getMaterial("GOLD"), textureSpecificationCube);
}

void Scene::onUIRender()
{
    ImGui::Begin("Scene Settings");

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

    // Light controls
    ImGui::Separator();

    ImGui::Text("Light Controls");

    ImGui::Separator();

    Light& light1 = m_lights[0];

    glm::vec3 lightPosition = light1.getPosition();
    if (ImGui::DragFloat3("Light Position: ", glm::value_ptr(lightPosition)))
        light1.setPosition(lightPosition);

    glm::vec4 lightColour = light1.getColour();
    if (ImGui::ColorPicker4("Light Colour: ", glm::value_ptr(lightColour)))
        light1.setColour(lightColour);

    ImGui::End();
}

void Scene::onWindowResize(uint32_t width, uint32_t height)
{
    m_camera.onWindowResize(width, height);
}

void Scene::setUpLights()
{
    Light::LightSpecification lightSpecification1;
    lightSpecification1.colour = { 1.0f, 1.0f, 1.0f, 1.0f };
    lightSpecification1.position = { 0.0f, 4.0f, 0.0f };

    m_lights[0].init(lightSpecification1);
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
    Renderer::drawCylinder(trunkTranslation * trunkRotation * trunkScale, MaterialLibrary::getMaterial("WOOD"), textureSpecificationTrunk);

    // Draw the tree canopy
    Renderer::CubeTextureSpecification textureSpecificationCanopy(&m_treeCanopyTexture);
    glm::mat4 canopyTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, 2.5f, 0.0f });
    glm::mat4 canopyScale = glm::scale(glm::mat4(1.0f), { 2.0f, 2.0f, 2.0f });
    Renderer::drawCube(canopyTranslation * canopyScale, MaterialLibrary::getMaterial("FAUNA"), textureSpecificationCanopy);

    glPopMatrix();
}
