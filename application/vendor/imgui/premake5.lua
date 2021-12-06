project "ImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	systemversion "latest"
	staticruntime "On"

	targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDirectory .. "/%{prj.name}")

	files
	{
		"imconfig.h",
		"imgui_demo.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_tables.cpp",
		"imgui_widgets.cpp",
		"imgui.cpp",
		"imgui.h",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		--- include specific files for the glfw backend ---
		"backends/imgui_impl_glfw.cpp",
		"backends/imgui_impl_glfw.h"
	}

	includedirs
	{
		".",
		"%{includeDirectories.GLFW}"
	}

	filter "system:linux"
		pic "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

