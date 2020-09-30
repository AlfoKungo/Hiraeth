project "Server"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.Cereal}",
		"%{IncludeDir.FreeImage}",
		"%{IncludeDir.Boost}",
		"%{IncludeDir.libpq}",
		"%{IncludeDir.SerializeLib}",
		"%{IncludeDir.MathLib}",
		"%{IncludeDir.NetworkLib}"
	}
	
	libdirs
	{
		"%{LibDir.FreeImage}",
		"%{LibDir.libpq}"
	}

	links
	{
		"libpq.lib",
		"MathLib",
		"SerializeLib"
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
