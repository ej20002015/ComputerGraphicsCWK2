project "application"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    systemversion "latest"

    targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDirectory .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.hpp",
        "src/**.cpp",
    }

    includedirs
    {
        "%{includeDirectories.GLFW}",
        "%{includeDirectories.Glad}",
        "%{includeDirectories.GLM}",
        "%{includeDirectories.ImGui}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui"
    }

    defines
    {
        "GLFW_INCLUDE_NONE" -- Include GLFW and OpenGL loader (glad) headers in any order by declaring this macro
    }

	filter "system:linux"

		links
		{
			"X11",
			"pthread",
			"dl",
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"