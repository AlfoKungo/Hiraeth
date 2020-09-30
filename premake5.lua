workspace "Hiraeth"
	architecture "x64"
	
workspace "Hiraeth"
	architecture "x86_64"
	startproject "Hiraeth"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "GladTest/vendor/GLFW/include"
IncludeDir["Glad"] = "GladTest/vendor/Glad/include"

include "GladTest/vendor/GLFW"
include "GladTest/vendor/Glad"

	
project "GladTest"
	location "GladTest"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
		
		-- "%{prj.name}/src/**.h",
		-- "%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}
	
	links
	{
		"GLFW",
		"opengl32.lib",
		"Glad",
	--	"opengl32.lib"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"HRTH_PLATFORM_WINDOWS",
		}
		
	filter "configurations:Debug"
		defines "HRTH_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "HRTH_RELEASE"
		symbols "On"
		
	filter "configurations:Dist"
		defines "HRTH_DIST"
		symbols "On"
		
	
	-- include
	-- {
		-- "%{prj.name}/vendor/spdlog/include"
	-- }