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

include "Callisto/vendor/GLFW"
include "Callisto/vendor/Glad"
include "Callisto/vendor/Imgui"

project "Callisto"
	location "Callisto"
	kind "SharedLib"
	language "C++"
	staticruntime "Off"

	targetdir ( "bin/" .. outputdir .. "/%{prj.name}")
	objdir ( "bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "CallistoPCH.h"
	pchsource "Callisto/src/CallistoPCH.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludedDirs.GLFW}",
		"%{IncludedDirs.Glad}",
		"%{IncludedDirs.Imgui}",
		"%{IncludedDirs.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"Imgui"
	}

	filter "system:windows"
		cppdialect "C++20"
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

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "CALLISTO_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CALLISTO_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CALLISTO_DIST"
		runtime "Release"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "Off"
	
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
		cppdialect "C++20"
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
		symbols "On"

	filter "configurations:Release"
		defines "CALLISTO_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CALLISTO_DIST"
		runtime "Release"
		optimize "On"