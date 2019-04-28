-- Premake file
--
-- This file is used to generate the Visual Studio 2017 solution and the
-- Makefile that we've included in the assignment. You will not need this file
-- under normal circumstances.
--
-- If you want to try to do the labs on an officially unsupported platform
-- (e.g., older Visual Studio), this file might be helpful, as you may be able
-- to generate solution/project/whatever files for your platform and IDE.  (We
-- have not tested this, however, so it might not work out-of-the-box.)
--
-- See https://premake.github.io/ for additional information. At the minimum
-- you will need to download the premake executable for your platform. 

workspace "IN4152_Practical01"
	platforms { "x64" }
	configurations { "debug", "release" }

	language "C++"

	cppdialect "C++14"
	flags "NoPCH"
	--vectorextensions "AVX"
	flags "MultiProcessorCompile"

	objdir "build/%{cfg.buildcfg}-%{cfg.platform}-%{cfg.toolset}"
	targetsuffix "-%{cfg.buildcfg}-%{cfg.platform}-%{cfg.toolset}"

	newoption {
		trigger = "toolset",
		description = "Select toolset on Linux / MacOS",
		allowed = {
			{ "gcc", "Use GCC" },
			{ "clang", "Use Clang" }
		}
	};

	-- Workaround empty "toolset"
	filter "system:linux or system:macos"
		toolset( _OPTIONS["toolset"] or "gcc" );
	filter "system:windows"
		toolset( "msc" );
	filter "*"

	-- default compiler flags
	filter "toolset:gcc or toolset:clang"
		linkoptions { "-pthread" }
		buildoptions { "-march=native", "-Wall", "-pthread" }

	filter "toolset:msc"
		defines { "_CRT_SECURE_NO_WARNINGS=1", "NOMINMAX=1" }

	filter "action:vs2015"
		buildoptions { "/utf-8" }
	
	filter "action:vs2017"
		buildoptions { "/utf-8" }
		--buildoptions { "/std:c++latest" }
	
	filter "*"

	-- default libraries
	filter "system:linux"
		links "GL"
		links "GLU"
		links "glut"

	filter "system:windows"
		links "OpenGL32"
		links "freeglut"

		libdirs "%{wks.location}/freeglut/lib/x64"
		includedirs "%{wks.location}/freeglut/include"

	filter "*"

	-- default outputs
	filter "kind:StaticLib"
		targetdir "lib/"

	filter "kind:ConsoleApp"
		targetdir "bin"
		targetextension ".exe"
	
	filter "*"

	--configurations
	configuration "debug"
		symbols "On" -- new, but doesn't work?
		--flags "Symbols" -- deprecated, but works
		defines { "_DEBUG=1" }

	configuration "release"
		optimize "On"
		defines { "NDEBUG=1" }
		flags "LinkTimeOptimization"

	configuration "*"

filter {"system:windows", "kind:ConsoleApp"}
	postbuildcommands {
		"{COPY} \"%{wks.location}/freeglut/bin/x64/freeglut.dll\" \"%{cfg.targetdir}\""
	}

	debugdir "%{wks.location}"

	filter "*"	
	
project( "Assignment" )
	kind "ConsoleApp"
	location "build/"

	local sources = { "*.cpp", "*.h", "*.hpp", "*.inl" }
	files( sources )

	
	--includedirs( 
	--links

--EOF
