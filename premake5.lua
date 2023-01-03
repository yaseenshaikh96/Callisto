workspace "Callisto"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludedDirs = {}
IncludedDirs["GLFW"] = "Callisto/vendor/GLFW/include"
IncludedDirs["Glad"] = "Callisto/vendor/Glad/include"
IncludedDirs["Imgui"] = "Callisto/vendor/Imgui"
IncludedDirs["glm"] = "Callisto/vendor/glm"
IncludedDirs["stb_image"] = "Callisto/vendor/stb_image"

include "Callisto/vendor/GLFW"
include "Callisto/vendor/Glad"
include "Callisto/vendor/Imgui"

project "Callisto"
	location "Callisto"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ( "bin/" .. outputdir .. "/%{prj.name}")
	objdir ( "bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "CallistoPCH.h"
	pchsource "Callisto/src/CallistoPCH.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",

		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h"

	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludedDirs.GLFW}",
		"%{IncludedDirs.Glad}",
		"%{IncludedDirs.Imgui}",
		"%{IncludedDirs.glm}",
		"%{IncludedDirs.stb_image}"

	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"Imgui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CALLISTO_PLATFORM_WINDOWS",
			"CALLISTO_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_WINDLL",
			"_UNICODE",
			"UNICODE"
		}

	filter "configurations:Debug"
		defines "CALLISTO_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CALLISTO_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CALLISTO_DIST"
		runtime "Release"
		optimize "on"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	targetdir ( "bin/" .. outputdir .. "/%{prj.name}")
	objdir ( "bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Callisto/vendor/spdlog/include",
		"Callisto/src",
		"Callisto/vendor",
		"%{IncludedDirs.glm}"
	}

	links
	{
		"Callisto"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CALLISTO_PLATFORM_WINDOWS",
			"_WINDLL",
			"_UNICODE",
			"UNICODE"
		}

	filter "configurations:Debug"
		defines "CALLISTO_DEBUG"
		runtime "Debug" 
		symbols "on"

	filter "configurations:Release"
		defines "CALLISTO_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CALLISTO_DIST"
		runtime "Release"
		optimize "on"