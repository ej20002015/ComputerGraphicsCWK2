#include "Material.h"

std::unordered_map<std::string, Material> MaterialLibrary::s_materials;

void MaterialLibrary::init()
{
    s_materials["WHITE_SHINY"] =
    {
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        100.0f
    };

    s_materials["BLACK_PLASTIC"] =
    {
        {   0.0f,   0.0f,   0.0f, 1.0f },
        { 0.001f, 0.001f, 0.001f, 1.0f },
        {   0.5f,   0.5f,   0.5f, 1.0f },
        32.0f
    };

    s_materials["GOLD"] =
    {
        {  0.24725f,   0.1995f,   0.0745f, 1.0f },
        {  0.75164f,  0.60648f,  0.22648f, 1.0f },
        { 0.628281f, 0.555802f, 0.366065f, 1.0f },
        51.2f
    };

    s_materials["NO_MATERIAL"] =
    {
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
        0.0f
    };

    s_materials["WOOD"] =
    {
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
        0.0f
    };

    s_materials["FAUNA"] =
    {
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 0.2f, 0.2f, 0.2f, 1.0f },
        1.0f
    };

    s_materials["GREEN_PLASTIC"] =
    {
        {  0.05f,  0.175f,  0.05f, 1.0f },
        {   0.1f,  0.35f,    0.1f, 1.0f },
        {  0.45f,  0.55f,   0.45f, 1.0f },
        32.0f
    };

    s_materials["RED_CLOTH"] =
    {
        { 0.25f, 0.0f, 0.0f, 1.0f },
        {  0.5f, 0.0f, 0.0f, 1.0f },
        {  0.3f, 0.0f, 0.0f, 1.0f },
        8.0f
    };

    s_materials["BLUE_CLOTH"] =
    {
        { 0.059f, 0.081f, 0.161f, 1.0f },
        { 0.118f, 0.161f, 0.322f, 1.0f },
        { 0.083f, 0.113f, 0.255f, 1.0f },
        8.0f
    };

    s_materials["SKIN"] =
    {
        { 0.953f, 0.765f,  0.69f, 1.0f },
        { 0.477f, 0.383f, 0.345f, 1.0f },
        { 0.286f, 0.115f, 0.104f, 1.0f },
        8.0f
    };

    s_materials["BLACK_LEATHER"] =
    {
        { 0.02f, 0.02f, 0.02f, 1.0f },
        { 0.01f, 0.01f, 0.01f, 1.0f },
        {  0.4f,  0.4f,  0.4f, 1.0f },
        32.0f
    };


    // Set to 5.0f to overcome the constant scene ambient value
    s_materials["WHITE_COLOUR"] =
    {
        { 5.0f, 5.0f, 5.0f, 5.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
        0.0f
    };

    s_materials["CHROME"] =
    {
        {     0.25f,     0.25f,     0.25f, 1.0f },
        {      0.4f,      0.4f,      0.4f, 1.0f },
        { 0.774597f, 0.774597f, 0.774597f, 1.0f },
        76.8f
    };
}

const Material& MaterialLibrary::getMaterial(const std::string& materialName)
{
    return s_materials.at(materialName);
}