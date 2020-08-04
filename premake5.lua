workspace "BirdLang"
	architecture "x64"
	startproject "Interpreter"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Compiler"
	location "Compiler"
	kind "StaticLib"
	staticruntime "on"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Compiler/src/pch.cpp"

	disablewarnings {
		"4141",
		"4146",
		"4244",
		"4267",
		"4624",
		"4996"
	}

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/include/**.hpp",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/grammar.txt",
		"Vendor/llvm/include/**.h",
		"Vendor/llvm/include/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/include",
		"Vendor/llvm/include"
	}

	--libdirs { 
		--"Vendor/llvm/lib"
	--}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		buildoptions{"/utf-8"}

		defines
		{
			"PLATFORM_WINDOWS"
		}

		--links {
			--"LLVMCore.lib"
		--}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PLATFORM_LINUX"
		}

	filter "configurations:Debug"
		defines {
			"DEBUG",
			'_ITERATOR_DEBUG_LEVEL=0'
		}
		symbols "On"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "On"

project "Interpreter"
	location "Interpreter"

	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Compiler/include",
		"Compiler/src",
	}

	links
	{
		"Compiler"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PLATFORM_WINDOWS"
		}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PLATFORM_LINUX"
		}

	filter "configurations:Debug"
		defines {
			"DEBUG",
			'_ITERATOR_DEBUG_LEVEL=0'
		}
		kind "ConsoleApp"
		symbols "On"
		
	filter "configurations:Release"
		defines "RELEASE"
		optimize "On"
		kind "ConsoleApp"
        
project "Tests"
	location "Tests"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
		"%{prj.name}/**.cc"
	}

	includedirs
	{
		"Compiler/src",
		"Vendor/googletest/include",
	}
	
	libdirs { 
		"Vendor/googletest/lib"
	}

	links
	{
		"Compiler"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PLATFORM_WINDOWS"
		}

		links {
			"gtestd.lib"
		}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PLATFORM_LINUX"
		}

		links {
			"pthread",
			"gtest:static"
		}

	filter "configurations:Debug"
		defines {
			"DEBUG",
		}
		kind "ConsoleApp"
		symbols "On"
		
	filter "configurations:Release"
		defines "RELEASE"
		optimize "On"
		kind "ConsoleApp"
		entrypoint "mainCRTStartup" 
