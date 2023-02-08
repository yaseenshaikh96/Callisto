include "premake5Dependencies.lua"

workspace "Callisto"
	architecture "x64"
	startproject "CallistoEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}
	

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


group "Core-Dependencies"
	include "Callisto/vendor/GLFW"
	include "Callisto/vendor/glad"
	include "Callisto/vendor/imgui"
	group ""


group "Core"
	include "Callisto"
	include "CallistoEditor"
group ""

group "Misc"
	include "SandBox"
group ""