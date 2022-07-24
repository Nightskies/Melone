project "Melone"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin_int/" .. outputdir .. "/%{prj.name}")

	pchheader "mlpch.h"
	pchsource "Src/mlpch.cpp"

	files
	{
		"Src/**.h",
		"Src/**.cpp",
		"Vendor/glm/glm/**.hpp",
		"Vendor/glm/glm/**.inl",
		"Vendor/stb_image/**.h",
		"Vendor/stb_image/**.cpp",
		"Vendor/ImGuizmo/ImGuizmo.h",
		"Vendor/ImGuizmo/ImGuizmo.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"Src",
		"Vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib"
	}

	filter "files:Vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
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