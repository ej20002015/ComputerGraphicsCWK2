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
        "vendor/stb_image/stb_image.cpp"
    }

    includedirs
    {
        "%{includeDirectories.GLFW}",
        "%{includeDirectories.Glad}",
        "%{includeDirectories.GLM}",
        "%{includeDirectories.ImGui}",
        "%{includeDirectories.stb_image}"
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
            "stdc++fs"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"