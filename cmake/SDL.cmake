include(ExternalProject)

set(SDL_VERSION "2.0.9")
set(SDL_VERSION_STRING "SDL2-${SDL_VERSION}")

set(SDL_ROOT_DIR "${EXTERN_DIR}/${SDL_VERSION_STRING}")
set(SDL_MSVC_DIR "${SDL_ROOT_DIR}/VisualC")

if(OS_WINDOWS)
	set(CONFIGURE_COMMAND "")
	set(BUILD_COMMAND
		MSBuild
			"${SDL_MSVC_DIR}/SDL.sln"
			/p:PlatformToolset=v142 # Default: v100
			/p:Configuration=${CMAKE_BUILD_TYPE}
			/p:Platform=x64
			/m)
	set(BUILD_BYPRODUCTS
		"${SDL_MSVC_DIR}/x64/${CMAKE_BUILD_TYPE}/SDL2.lib"
		"${SDL_MSVC_DIR}/x64/${CMAKE_BUILD_TYPE}/SDL2main.lib")

	install(
		FILES "${SDL_MSVC_DIR}/x64/${CMAKE_BUILD_TYPE}/SDL2.dll"
		DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()

if(OS_MACOSX)
	set(CONFIGURE_COMMAND ./configure)
	set(BUILD_COMMAND make
		COMMAND install_name_tool
			-id
			"@executable_path/../Frameworks/libSDL2-2.0.0.dylib"
			"${SDL_ROOT_DIR}/build/.libs/libSDL2-2.0.0.dylib")
	set(BUILD_BYPRODUCTS
		"${SDL_ROOT_DIR}/build/.libs/libSDL2-2.0.0.dylib"
		"${SDL_ROOT_DIR}/build/.libs/libSDL2main.a")

	install(
		FILES "${SDL_ROOT_DIR}/build/.libs/libSDL2-2.0.0.dylib"
		DESTINATION "${CMAKE_INSTALL_PREFIX}/CompositeEngine.app/Contents/Frameworks")
endif()

ExternalProject_Add(
	SDLExternal
	PREFIX ${SDL_VERSION_STRING}

	DOWNLOAD_DIR ${EXTERN_DIR}
	URL "https://www.libsdl.org/release/${SDL_VERSION_STRING}.tar.gz"

	SOURCE_DIR ${SDL_ROOT_DIR}
	BINARY_DIR ${SDL_ROOT_DIR}

	CONFIGURE_COMMAND "${CONFIGURE_COMMAND}"
	BUILD_COMMAND "${BUILD_COMMAND}"
	INSTALL_COMMAND ""

	BUILD_BYPRODUCTS ${BUILD_BYPRODUCTS}
)

add_library(SDL INTERFACE)
add_dependencies(SDL SDLExternal)
target_include_directories(SDL INTERFACE "${SDL_ROOT_DIR}/include")
target_link_libraries(SDL INTERFACE ${BUILD_BYPRODUCTS})
