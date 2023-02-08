project "Callisto"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ( "%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ( "%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "CallistoPCH.h"
	pchsource "Callisto/src/CallistoPCH.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",

		"%{wks.location}/%{prj.name}/vendor/glm/glm/**.hpp",
		"%{wks.location}/%{prj.name}/vendor/glm/glm/**.inl",

		"%{wks.location}/%{prj.name}/vendor/stb_image/**.cpp",
		"%{wks.location}/%{prj.name}/vendor/stb_image/**.h"

	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludedDirs.glad}",
		"%{IncludedDirs.GLFW}",
		"%{IncludedDirs.imgui}",
		"%{IncludedDirs.glm}",
		"%{IncludedDirs.stb_image}",
		"%{IncludedDirs.entt}"
	}

	links
	{
		"glad",
		"GLFW",
		"imgui",
		"opengl32.lib"
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