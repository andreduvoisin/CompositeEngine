include(ExternalProject)

set(SDL_VERSION "2.0.9")
set(SDL_VERSION_STRING "SDL2-${SDL_VERSION}")

set(SDL_ROOT_DIR "${EXTERN_DIR}/${SDL_VERSION_STRING}")
set(SDL_MSVC_DIR "${SDL_ROOT_DIR}/VisualC")

if(${CE_CONFIGURATION} STREQUAL "Debug")
	set(SDL_CONFIGURATION "Debug")
elseif(${CE_CONFIGURATION} STREQUAL "Release")
	set(SDL_CONFIGURATION "Release")
endif()

if(OS_WINDOWS)
	if(${CE_PLATFORM} STREQUAL "Win32")
		set(SDL_PLATFORM "Win32")
	elseif(${CE_PLATFORM} STREQUAL "x64")
		set(SDL_PLATFORM "x64")
	endif()
endif()

function(BuildSDL)
	if(OS_WINDOWS)
		set(CONFIGURE_COMMAND "")
		set(BUILD_COMMAND
			MSBuild
				"${SDL_MSVC_DIR}/SDL.sln"
				/p:PlatformToolset=v141 # Default: v100
				/p:Configuration=${CE_CONFIGURATION}
				/p:Platform=${CE_PLATFORM}
				/m
		)
		set(BUILD_BYPRODUCTS
			"${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2.lib"
			"${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2main.lib"
		)
	elseif(OS_MACOSX)
		set(CONFIGURE_COMMAND ./configure)
		set(BUILD_COMMAND make
			COMMAND install_name_tool -id "@executable_path/../Frameworks/libSDL2.dylib" "${SDL_ROOT_DIR}/build/.libs/libSDL2.dylib"
		)
		set(BUILD_BYPRODUCTS
			"${SDL_ROOT_DIR}/build/.libs/libSDL2.dylib"
			"${SDL_ROOT_DIR}/build/.libs/libSDL2main.a"
		)
	endif()

	ExternalProject_Add(
		SDL
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
endfunction()

function(BootstrapSDL TARGET_NAME EXECUTABLE_SUBDIR)
	IncludeSDL()
	LinkSDL(${TARGET_NAME})
	CopySDLFiles(${TARGET_NAME} ${EXECUTABLE_SUBDIR})
endfunction()

function(IncludeSDL)
	include_directories("${SDL_ROOT_DIR}/include")
endfunction()

function(LinkSDL TARGET_NAME)
	if(OS_WINDOWS)
		set(LIBRARIES
			"${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2.lib"
			"${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2main.lib"
		)
	elseif(OS_MACOSX)
		set(LIBRARIES
			"${SDL_ROOT_DIR}/build/.libs/libSDL2.dylib"
			"${SDL_ROOT_DIR}/build/.libs/libSDL2main.a"
		)
	endif()
	
	target_link_libraries(${TARGET_NAME} PRIVATE ${LIBRARIES})
endfunction()

function(CopySDLFiles TARGET_NAME EXECUTABLE_SUBDIR)
	if(OS_WINDOWS)
		set(LIBRARY_SRC "${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2.dll")
		set(LIBRARY_DST "${PROJECT_BINARY_DIR}/${EXECUTABLE_SUBDIR}/${CE_CONFIGURATION}/SDL2.dll")
	elseif(OS_MACOSX)
		set(LIBRARY_SRC "${SDL_ROOT_DIR}/build/.libs/libSDL2.dylib")
		set(LIBRARY_DST "${PROJECT_BINARY_DIR}/engine/${CE_CONFIGURATION}/CompositeEngine.app/Contents/Frameworks/libSDL2.dylib")
	endif()

	add_custom_command(
		TARGET ${TARGET_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
			${LIBRARY_SRC}
			${LIBRARY_DST}
		VERBATIM
	)
endfunction()
