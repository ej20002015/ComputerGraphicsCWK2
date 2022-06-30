#pragma once

#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

struct Material
{
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float shininess;
};

class MaterialLibrary
{
public:

    MaterialLibrary() = default;
    static void init();

    static const Material& getMaterial(const std::string& materialName);

private:
    
    static std::unordered_map<std::string, Material> s_materials;
};