project "NetworkLib"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"net/**.h",
		"net/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"net",
		"%{IncludeDir.Cereal}",
		"%{IncludeDir.MathLib}",	
		"%{IncludeDir.SerializeLib}"
	}

	libdirs
	{
		"%{LibDir.FreeImage}"
	}
	
	links
	{
		"FreeImage64.lib",
		"MathLib"
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
		}

	filter "configurations:Debug"
		defines "HRTH_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HRTH_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HRTH_DIST"
		runtime "Release"
		optimize "on"
