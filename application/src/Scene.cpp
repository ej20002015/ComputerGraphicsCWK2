#include "Scene.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "Renderer.h"
#include "Application.h"
#include "Material.h"
#include "Log.h"
#include "UserInterface.h"

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

void Scene::onUpdate(float timeStep)
{
    // Set the background to a blueish sky colour
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

    // Light positions needs to be updated per frame since it is dependent on the modelview matrix

    for (auto light : m_lights)
        light.update();

    // See light locations

    if (m_showLightPositions)
    {
        for (auto light : m_lights)
            light.renderLocation();
    }

    // Render some ground
    
    Renderer::CubeTextureSpecification textureSpecificationGround;
    textureSpecificationGround.topFace = &m_grassTexture;
    Renderer::drawCube(glm::scale(glm::mat4(1.0f), { 50.0f, 0.5f, 50.0f }), MaterialLibrary::getMaterial("FAUNA"), textureSpecificationGround, 25.0f);

    // Draw the forest
    drawForest();

    // Draw the table along with the spinning top and map
    drawTable(glm::translate(glm::mat4(1.0f), { -3.0f, 0.0f, -1.0f }), timeStep);

    // Draw the lumberjacks
    drawLumberjacksScene(glm::translate(glm::mat4(1.0f), { 2.0f, 0.0f, 0.0f }), timeStep);
}

void Scene::onUIRender()
{
    ImGui::Begin("Scene Settings");

    // Camera settings

    ImGui::Separator();
    UserInterface::pushBoldFont();
    ImGui::Text("Camera Settings - Hold Right Click to Manipulate Camera");
    UserInterface::popBoldFont();
    ImGui::Separator();

    bool cameraLockYAxisPosition = m_camera.getLockYAxisPosition();
    if (ImGui::Checkbox("Lock Camera Y Axis Position", &cameraLockYAxisPosition))
        m_camera.setLockYAxisPosition(cameraLockYAxisPosition);

    float cameraSpeed = m_camera.getSpeed();
    if (ImGui::DragFloat("Camera Speed", &cameraSpeed, 0.1f, 0.2f, 10.0f))
        m_camera.setSpeed(cameraSpeed);

    float cameraSensitivity = m_camera.getSensitivity();
    if (ImGui::DragFloat("Camera Sensitivity", &cameraSensitivity, 0.01f, 0.02f, 0.4f))
        m_camera.setSensitivity(cameraSensitivity);

    // Light settings
    
    ImGui::Separator();
    UserInterface::pushBoldFont();
    std::string lightSettingsHeader = std::string("Light Settings - Modifying Light ") + std::to_string(m_selectedLight);
    ImGui::Text("%s", lightSettingsHeader.c_str());
    UserInterface::popBoldFont();
    ImGui::Separator();

    // Make a drop down so the user can choose which of the 3 lights to modify

    const char* lightDropDownLabels[3] =
    {
        "Light 0",
        "Light 1",
        "Light 2"
    };

    const char* selectedDropDownLabel = lightDropDownLabels[m_selectedLight];

    if (ImGui::BeginCombo("Select Light To Edit", selectedDropDownLabel, ImGuiComboFlags_NoArrowButton))
    {
        for (uint32_t i = 0; i < 3; i++)
        {
            bool isSelected = selectedDropDownLabel == lightDropDownLabels[i];
            if (ImGui::Selectable(lightDropDownLabels[i], isSelected))
                m_selectedLight = i;
            
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    Light& light = m_lights[m_selectedLight];
    ImGui::Separator();

    glm::vec3 lightPosition = light.getPosition();
    if (ImGui::DragFloat3("Light Position", glm::value_ptr(lightPosition), 0.5f))
        light.setPosition(lightPosition);

    glm::vec4 lightColour = light.getColour();
    if (ImGui::ColorPicker4("Light Colour", glm::value_ptr(lightColour)))
        light.setColour(lightColour);

    ImGui::Checkbox("Show Light Positions", &m_showLightPositions);

    // Spinning top settings

    ImGui::Separator();
    UserInterface::pushBoldFont();
    ImGui::Text("Spinning Top Settings");
    UserInterface::popBoldFont();
    ImGui::Separator();

    ImGui::DragFloat("Spinning Top Speed", &m_spinningTopRotationSpeed, 25.0f, -1000.0f, 1000.0f);

    // Lumberjacks settings

    ImGui::Separator();
    UserInterface::pushBoldFont();
    ImGui::Text("Lumberjack Settings");
    UserInterface::popBoldFont();
    ImGui::Separator();

    float lumberjackChopSpeed = m_lumberjack1.getAnimationSpeed();
    if (ImGui::DragFloat("Lumberjacks Chopping Speed", &lumberjackChopSpeed, 0.05f, -2.0f, 2.0f))
    {
        m_lumberjack1.setAnimationSpeed(lumberjackChopSpeed);
        m_lumberjack2.setAnimationSpeed(lumberjackChopSpeed);
    }

    ImGui::DragFloat("Lumberjacks Rotation Speed", &m_lumberjacksRotationSpeed, 5.0f, -200.0f, 200.0f);

    ImGui::End();
}

void Scene::onWindowResize(uint32_t width, uint32_t height)
{
    m_camera.onWindowResize(width, height);
}

void Scene::setUpLights()
{
    Light::LightSpecification lightSpecification1, lightSpecification2, lightSpecification3;
    lightSpecification1.position = {  0.0f, 6.0f, 0.0f };
    lightSpecification2.position = { -6.0f, 6.0f, 0.0f };
    lightSpecification3.position = {  6.0f, 6.0f, 0.0f };

    m_lights[0].init(lightSpecification1);
    m_lights[1].init(lightSpecification2);
    m_lights[2].init(lightSpecification3);
}

void Scene::setUpTextures()
{
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

    Texture::TextureSpecification tableTopTextureSpecification;
    tableTopTextureSpecification.filepath = "assets/textures/tableTop.jpg";
    m_tableTopTexture.init(tableTopTextureSpecification);

    Texture::TextureSpecification tableLegTextureSpecification;
    tableLegTextureSpecification.filepath = "assets/textures/tableLeg.jpg";
    m_tableLegTexture.init(tableLegTextureSpecification);
    m_lumberjack1.setAxHandleTexture(&m_tableLegTexture);
    m_lumberjack2.setAxHandleTexture(&m_tableLegTexture);

    Texture::TextureSpecification mapTextureSpecifcation;
    mapTextureSpecifcation.filepath = "assets/textures/Mercator-projection.jpg";
    m_mapTexture.init(mapTextureSpecifcation);

    Texture::TextureSpecification markusTextureSpecification;
    markusTextureSpecification.filepath = "assets/textures/markus.jpg";
    m_markusTexture.init(markusTextureSpecification);
    m_lumberjack1.setFaceTexture(&m_markusTexture);

    Texture::TextureSpecification marcTextureSpecification;
    marcTextureSpecification.filepath = "assets/textures/Marc_Dekamps.jpg";
    m_marcTexture.init(marcTextureSpecification);
    m_lumberjack2.setFaceTexture(&m_marcTexture);
}

void Scene::drawForest()
{
    // Just hard coding some positions and rotations of the trees of the forest

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

void Scene::drawLumberjacksScene(const glm::mat4& transform, float timeStep)
{
    // Apply the transform

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glMultMatrixf(glm::value_ptr(transform));

    // Draw the tree that is being chopped in the center
    drawTree(glm::mat4(1.0f));

    // Draw some nearby logs that have already been chopped down

    Renderer::CylinderTextureSpecification logCylinderTextureSpecification;
    logCylinderTextureSpecification.frontFace = &m_treeTrunkCrossSectionTexture;
    logCylinderTextureSpecification.curvedFace = &m_barkTexture;
    logCylinderTextureSpecification.backFace = &m_treeTrunkCrossSectionTexture;

    glm::mat4 logRotation = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), { 0.0f, 1.0f, 0.0f });
    glm::mat4 logScale = glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 1.5f });

    glm::mat4 log1Translation = glm::translate(glm::mat4(1.0f), { 3.0f, 0.5f, 2.0f });
    glm::mat4 log2Translation = glm::translate(glm::mat4(1.0f), { 4.0f, 0.5f, 2.0f });

    Renderer::drawCylinder(log1Translation * logRotation * logScale, MaterialLibrary::getMaterial("WOOD"), logCylinderTextureSpecification);
    Renderer::drawCylinder(log2Translation * logRotation * logScale, MaterialLibrary::getMaterial("WOOD"), logCylinderTextureSpecification);
    
    // Draw the lumberjacks

    // Calculate the position of the lumberjacks so they rotate around the center point (the tree)

    const float circleRadius = 0.84f;
    m_lumberjacksRotationAngle = glm::mod<float>(m_lumberjacksRotationAngle + (timeStep * m_lumberjacksRotationSpeed), 360.0f);
    // Using polar coordinates to get positions on a circle around the tree
    glm::vec3 lumberjack1Position = { circleRadius * glm::sin(glm::radians(m_lumberjacksRotationAngle)), 0.0f, circleRadius * glm::cos(glm::radians(m_lumberjacksRotationAngle)) };
    glm::vec3 lumberjack2Position = -lumberjack1Position;
    glm::mat4 lumberjack1Translation = glm::translate(glm::mat4(1.0f), { lumberjack1Position });
    glm::mat4 lumberjack2Translation = glm::translate(glm::mat4(1.0f), { lumberjack2Position });

    // Calculate the rotation of the lumberjacks so there always facing the center of the circle

    glm::mat4 lumberjack1Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_lumberjacksRotationAngle - 180.0f), { 0.0f, 1.0f, 0.0f });
    glm::mat4 lumberjack2Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_lumberjacksRotationAngle), { 0.0f, 1.0f, 0.0f });

    m_lumberjack1.setTransform(lumberjack1Translation * lumberjack1Rotation);
    m_lumberjack1.update(timeStep);

    m_lumberjack2.setTransform(lumberjack2Translation * lumberjack2Rotation);
    m_lumberjack2.update(timeStep);

    // Restore the state of the MODELVIEW matrix
    glPopMatrix();
}

void Scene::drawTable(const glm::mat4& transform, float timeStep)
{
    // Apply the transform

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glMultMatrixf(glm::value_ptr(transform));

    // Draw table

    // Draw the table top

    Renderer::CubeTextureSpecification tableTopCubeTextureSpecification(&m_tableTopTexture);

    glm::mat4 tableTopTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, 0.8f, 0.0f });
    glm::mat4 tableTopScale = glm::scale(glm::mat4(1.0f), { 3.0f, 0.1f, 1.0f });

    Renderer::drawCube(tableTopTranslation * tableTopScale, MaterialLibrary::getMaterial("WOOD"), tableTopCubeTextureSpecification);

    // Draw the table legs

    Renderer::CylinderTextureSpecification tableLegsCylinderTextureSpecification(&m_tableLegTexture);

    glm::mat4 tableLegRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
    glm::mat4 tableLegScale = glm::scale(glm::mat4(1.0f), { 0.1f, 0.1f, 0.85f });

    glm::mat4 tableLegBackLeftTranslation   = glm::translate(glm::mat4(1.0f), { -1.40f, 0.42f, -0.4f });
    glm::mat4 tableLegFrontLeftTranslation  = glm::translate(glm::mat4(1.0f), { -1.40f, 0.42f,  0.4f });
    glm::mat4 tableLegBackRightTranslation  = glm::translate(glm::mat4(1.0f), {  1.40f, 0.42f, -0.4f });
    glm::mat4 tableLegFrontRightTranslation = glm::translate(glm::mat4(1.0f), {  1.40f, 0.42f,  0.4f });

    Renderer::drawCylinder(tableLegBackLeftTranslation * tableLegRotation * tableLegScale, MaterialLibrary::getMaterial("WOOD"), tableLegsCylinderTextureSpecification);
    Renderer::drawCylinder(tableLegFrontLeftTranslation * tableLegRotation * tableLegScale, MaterialLibrary::getMaterial("WOOD"), tableLegsCylinderTextureSpecification);
    Renderer::drawCylinder(tableLegBackRightTranslation * tableLegRotation * tableLegScale, MaterialLibrary::getMaterial("WOOD"), tableLegsCylinderTextureSpecification);
    Renderer::drawCylinder(tableLegFrontRightTranslation * tableLegRotation * tableLegScale, MaterialLibrary::getMaterial("WOOD"), tableLegsCylinderTextureSpecification);

    // Draw spinning top

    glm::mat4 spinningTopTranslation = glm::translate(glm::mat4(1.0f), { 1.0f, 0.9f, 0.4f });
    // Applying the modulus operator to the rotation angle so an overflow does not occur
    // Angle goes smoothly from 0 to 360, and then immediately back to 0 again
    m_spinningTopRotationAngle = glm::mod<float>(m_spinningTopRotationAngle + (timeStep * m_spinningTopRotationSpeed), 360.0f);
    glm::mat4 spinningTopRotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_spinningTopRotationAngle), { 0.0f, 1.0f, 0.0f });

    glPushMatrix();

    // Apply rotation and translation to the spinning top body and handle
    glMultMatrixf(glm::value_ptr(spinningTopTranslation * spinningTopRotation));

    // Draw the spinning top main body

    glm::mat4 spinningTopBodyScale = glm::scale(glm::mat4(1.0f), { 0.05f, 0.1f, 0.05f });

    Renderer::drawOctahedron(spinningTopBodyScale, MaterialLibrary::getMaterial("GREEN_PLASTIC"));

    // Draw the spinning top handle

    glm::mat4 spinningTopHandleTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, 0.06f, 0.0f });
    glm::mat4 spinningTopHandleRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
    glm::mat4 spinningTopHandleScale = glm::scale(glm::mat4(1.0f), { 0.01f, 0.01f, 0.05f });

    Renderer::drawCylinder(spinningTopHandleTranslation * spinningTopHandleRotation * spinningTopHandleScale, MaterialLibrary::getMaterial("GREEN_PLASTIC"));

    glPopMatrix();

    // Draw a map of the world

    Renderer::CubeTextureSpecification mapCubeTextureSpecification;
    mapCubeTextureSpecification.topFace = &m_mapTexture;

    glm::mat4 mapTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, 0.85f, 0.0f });
    glm::mat4 mapScale = glm::scale(glm::mat4(1.0f), { 1.1f, 0.05f, 0.75f });

    Renderer::drawCube(mapTranslation * mapScale, MaterialLibrary::getMaterial("WOOD"), mapCubeTextureSpecification);

    // Restore the state of the MODELVIEW matrix
    glPopMatrix();
}

void Scene::drawTree(const glm::mat4& transform)
{
    // Apply the transform

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glMultMatrixf(glm::value_ptr(transform));

    // Draw the trunk of the tree

    Renderer::CylinderTextureSpecification trunkCylinderTextureSpecification;
    trunkCylinderTextureSpecification.frontFace = &m_treeTrunkCrossSectionTexture;
    trunkCylinderTextureSpecification.curvedFace = &m_barkTexture;
    trunkCylinderTextureSpecification.backFace = &m_treeTrunkCrossSectionTexture;

    glm::mat4 trunkTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, 1.4f, 0.0f });
    glm::mat4 trunkRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
    glm::mat4 trunkScale = glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 3.0f });

    Renderer::drawCylinder(trunkTranslation * trunkRotation * trunkScale, MaterialLibrary::getMaterial("WOOD"), trunkCylinderTextureSpecification);

    // Draw the tree canopy

    Renderer::CubeTextureSpecification canopyCubeTextureSpecification(&m_treeCanopyTexture);

    glm::mat4 canopyTranslation = glm::translate(glm::mat4(1.0f), { 0.0f, 2.5f, 0.0f });
    glm::mat4 canopyScale = glm::scale(glm::mat4(1.0f), { 2.0f, 2.0f, 2.0f });

    Renderer::drawCube(canopyTranslation * canopyScale, MaterialLibrary::getMaterial("FAUNA"), canopyCubeTextureSpecification);

    // Restore the state of the MODELVIEW matrix
    glPopMatrix();
}
