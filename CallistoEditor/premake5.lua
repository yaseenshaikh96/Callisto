project "CallistoEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	targetdir ( "%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ( "%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"%{wks.location}/Callisto/vendor/spdlog/include",
		"%{wks.location}/Callisto/src",
		"%{wks.location}/Callisto/vendor",
		"%{IncludedDirs.glm}",
		"%{IncludedDirs.entt}",
		"%{IncludedDirs.ImGuizmo}"
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