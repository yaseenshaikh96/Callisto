workspace "Callisto"
	architecture "x64"

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

include "Callisto/vendor/GLFW"
include "Callisto/vendor/Glad"
include "Callisto/vendor/Imgui"

project "Callisto"
	location "Callisto"
	kind "SharedLib"
	language "C++"
	targetdir ( "bin/" .. outputdir .. "/%{prj.name}")
	objdir ( "bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "CallistoPCH.h"
	pchsource "Callisto/src/CallistoPCH.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludedDirs.GLFW}",
		"%{IncludedDirs.Glad}",
		"%{IncludedDirs.Imgui}"
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
		staticruntime "On"
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
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "CALLISTO_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "CALLISTO_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "CALLISTO_DIST"
		buildoptions "/MD"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
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
		"Callisto/src"
	}

	links
	{
		"Callisto"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
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
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "CALLISTO_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "CALLISTO_DIST"
		buildoptions "/MD"
		optimize "On"