project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin_int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Src/**.h",
		"Src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Vendor/spdlog/include",
		"%{wks.location}/Src",
		"%{wks.location}/Vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
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