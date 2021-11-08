workspace "Hiraeth"
	architecture "x64"
	startproject "Hiraeth"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	

	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Dependencies/GLFW/include"
IncludeDir["GLEW"] = "%{wks.location}/Dependencies/GLEW/include"
IncludeDir["Glad"] = "%{wks.location}/Dependencies/Glad/include"
IncludeDir["FreeType"] = "%{wks.location}/Dependencies/FreeType/include"
IncludeDir["Cereal"] = "%{wks.location}/Dependencies/Cereal/include"
IncludeDir["FreeImage"] = "%{wks.location}/Dependencies/FreeImage/include"
IncludeDir["libpq"] = "%{wks.location}/Dependencies/libpq/include"
IncludeDir["Boost"] = "%{wks.location}/Dependencies/Boost/include"
IncludeDir["spdlog"] = "%{wks.location}/submodules/spdlog/include"
IncludeDir["MathLib"] = "%{wks.location}/MathLib"
IncludeDir["SerializeLib"] = "%{wks.location}/SerializeLib"
IncludeDir["NetworkLib"] = "%{wks.location}/NetworkLib"

LibDir = {}
LibDir["FreeImage"] = "%{wks.location}/Dependencies/FreeImage/lib"
LibDir["FreeType"] = "%{wks.location}/Dependencies/FreeType/lib"
LibDir["libpq"] = "%{wks.location}/Dependencies/libpq/lib"
-- LibDir["GLFW"] = "%{wks.location}/Dependencies/GLFW/lib-vc2019_64"
LibDir["GLEW"] = "%{wks.location}/Dependencies/GLEW/lib"

group "Dependencies"
	include "Dependencies/GLFW"
	-- include "Dependencies/Glad"

group ""
	include "MathLib"
	include "SerializeLib"
	include "NetworkLib"
	include "Hiraeth"
	include "Server"
	include "Serializer"

-- include "GladTest/vendor/GLFW"
-- include "GladTest/vendor/Glad"
