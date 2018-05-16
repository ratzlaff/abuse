require "xcode"

function default_sdl_build(inDir)
	location "."
	kind "StaticLib"
	language "C++"

	targetdir "../bin"
	defines { "HAVE_CONFIG_H", "PACKAGE_NAME=\"abuse\"", "PACKAGE_VERSION=\"0.9\"" }

	includedirs { "../src", "../src/imlib" }
	files {"../src/" .. inDir .. "/*.cpp", "../src/" .. inDir .. "/*.h"}

	filter "system:macosx"
		includedirs { "/usr/local/include/SDL2", "../osx" }
	filter {}
end

workspace "abuse"
	configurations {"Debug", "Release"}
	location ".."
	startproject "abuse"

	project "abuse-tool"
		location "."
		kind "ConsoleApp"
		language "C++"

		targetdir "../bin"
		defines { "HAVE_CONFIG_H", "PACKAGE_NAME=\"abuse\"", "PACKAGE_VERSION=\"0.9\"" }

		includedirs { "../src", "../src/imlib", "../src/lisp", "../src/net" }
		files { "../src/tool/*.cpp", "../src/tool/*.h", "../src/crc.cpp" }
		links { "lisp", "net", "imlib", "sdlport" }

		filter "system:macosx"
			includedirs { "../osx" }
		filter {}

	project "abuse"
		location "."
		kind "ConsoleApp"
		language "C++"

		targetdir "../bin"
		defines { "HAVE_CONFIG_H", "PACKAGE_NAME=\"abuse\"", "PACKAGE_VERSION=\"0.9\"" }

		includedirs { "../src", "../src/imlib", "../src/lisp", "../src/net" }
		files { "../src/*.cpp", "../src/*.h", "../src/ui/*.cpp", "../src/ui/*.h", "../src/lol/*.cpp", "../src/lol/*.h" }

		links { "lisp", "net", "imlib", "sdlport", "SDL2", "SDL2_mixer" }

		filter "system:macosx"
			kind "WindowedApp"
			files { "../osx/Info.plist" }
			includedirs { "/usr/local/include/SDL2", "../osx" }
			libdirs { "/usr/local/lib" } 
			linkoptions { "-framework Cocoa" }
		filter {}

	project "lisp"
		default_sdl_build("lisp")

	project "net"
		default_sdl_build("net")

	project "imlib"
		default_sdl_build("imlib")

	project "sdlport"
		default_sdl_build("sdlport")
