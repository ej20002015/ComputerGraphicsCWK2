workspace "CWK2"
    architecture "x86_64"
    startproject "application"

    includeDirectories = {}
    includeDirectories["GLFW"] = "%{wks.location}/application/vendor/glfw/include/GLFW"
    includeDirectories["Glad"] = "%{wks.location}/application/vendor/glad/include"
    includeDirectories["GLM"] = "%{wks.location}/application/vendor/glm"

    configurations
    {
        "Debug",
        "Release"
    }

    outputDirectory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    include "application"
    include "application/vendor/glfw"
    include "application/vendor/glad"
