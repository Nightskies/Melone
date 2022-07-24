workspace "Melone"
	architecture "x86_64"
	startproject "MeloneEditor"

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

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Melone/Vendor/GLFW/include"
IncludeDir["glm"] = "%{wks.location}/Melone/Vendor/glm"
IncludeDir["Glad"] = "%{wks.location}/Melone/Vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Melone/Vendor/imgui"
IncludeDir["stb_image"] = "%{wks.location}/Melone/Vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Melone/Vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Melone/Vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Melone/Vendor/ImGuizmo"

group "Dependencies"
	include "Melone/Vendor/GLFW"
	include "Melone/Vendor/Glad"
	include "Melone/Vendor/imgui"
	include "Melone/Vendor/yaml-cpp"
group ""

include "Melone"
include "Sandbox"
include "MeloneEditor"