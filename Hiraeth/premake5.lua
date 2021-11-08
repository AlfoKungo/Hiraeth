project "Hiraeth"
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
		"ext/freetype-gl/**"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
		"GLEW_STATIC",
		"WIN64",
		"_LIB",
		"_CRT_NONSTDC_NO_DEPRECATE"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.FreeType}",
		"%{IncludeDir.Cereal}",
		"%{IncludeDir.FreeImage}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.GLFW}",
		-- "%{IncludeDir.Glad}",
		-- "%{IncludeDir.Boost}",
		-- "%{IncludeDir.libpq}",
		"%{IncludeDir.SerializeLib}",
		"%{IncludeDir.MathLib}",
		"%{IncludeDir.NetworkLib}",
		"%{IncludeDir.spdlog}"
	}
	
	libdirs
	{
		"D:/Program Files (x86)/Windows Kits/10/Lib/10.0.15063.0/um/x64",
		"D:/Program Files (x86)/Microsoft Visual Studio/VC/Tools/MSVC/14.13.26128/lib/onecore/x64",
		"D:/Program Files (x86)/Windows Kits/10/Lib/10.0.15063.0/ucrt/x64",
		"%{LibDir.FreeImage}",
		"%{LibDir.FreeType}",
		-- "%{LibDir.GLFW}",
		"%{LibDir.GLEW}"
	}

	links
	{
		"freetype_64.lib",
		-- "glfw3.lib",
		"GLFW",
	--	"Glad",
		"glew64s.lib",
		"opengl32.lib",
		"FreeImage64.lib",
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
