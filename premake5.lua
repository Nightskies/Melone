workspace "Melone"
	architecture "x86_64"
	startproject "Sandbox"

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
IncludeDir["GLFW"] = "Melone/Vendor/GLFW/include"
IncludeDir["glm"] = "Melone/Vendor/glm"
IncludeDir["Glad"] = "Melone/Vendor/Glad/include"
IncludeDir["ImGui"] = "Melone/Vendor/imgui"

group "Dependencies"
	include "Melone/Vendor/GLFW"
	include "Melone/Vendor/Glad"
	include "Melone/Vendor/imgui"

group ""

project "Melone"
	location "Melone"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin_int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp",
		"%{prj.name}/Vendor/glm/glm/**.hpp",
		"%{prj.name}/Vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/Src",
		"%{prj.name}/Vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "MELONE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MELONE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MELONE_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin_int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp"
	}

	includedirs
	{
		"Melone/Vendor/spdlog/include",
		"Melone/Src",
		"Melone/Vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Melone"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "MELONE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MELONE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MELONE_DIST"
		runtime "Release"
		optimize "on"