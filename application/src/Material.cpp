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


    // Set to 5.0f to overcome the constant scene ambient value
    s_materials["WHITE_COLOUR"] =
    {
        { 5.0f, 5.0f, 5.0f, 5.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
        0.0f
    };
}

const Material& MaterialLibrary::getMaterial(const std::string& materialName)
{
    return s_materials.at(materialName);
}