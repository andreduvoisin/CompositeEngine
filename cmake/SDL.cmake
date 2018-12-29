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

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	if(${CE_PLATFORM} STREQUAL "Win32")
		set(SDL_PLATFORM "Win32")
	elseif(${CE_PLATFORM} STREQUAL "x64")
		set(SDL_PLATFORM "x64")
	endif()
endif()

function(BuildSDL)
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		execute_process(
			COMMAND
				MSBuild
				"${SDL_MSVC_DIR}/SDL.sln"
				/p:PlatformToolset=v141 # Default: v100
				/p:Configuration=${CE_CONFIGURATION}
				/p:Platform=${CE_PLATFORM}
				/m
		)
	elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
		# execute_process(COMMAND ./configure WORKING_DIRECTORY "${SDL_ROOT_DIR}")
		# execute_process(COMMAND make WORKING_DIRECTORY "${SDL_ROOT_DIR}")
		# execute_process(
		# 	COMMAND install_name_tool -id
		# 		"@executable_path/../Frameworks/libSDL2.dylib"
		# 		"${SDL_ROOT_DIR}/build/.libs/libSDL2.dylib"
		# )
		ExternalProject_Add(
			SDL
			PREFIX ${SDL_VERSION_STRING}

			DOWNLOAD_DIR ${EXTERN_DIR}
			URL "https://www.libsdl.org/release/${SDL_VERSION_STRING}.tar.gz"

			SOURCE_DIR ${SDL_ROOT_DIR}
			BINARY_DIR ${SDL_ROOT_DIR}

			CONFIGURE_COMMAND ./configure
			BUILD_COMMAND make
				COMMAND install_name_tool -id "@executable_path/../Frameworks/libSDL2.dylib" "${SDL_ROOT_DIR}/build/.libs/libSDL2.dylib"
			INSTALL_COMMAND ""

			BUILD_BYPRODUCTS
				"${SDL_ROOT_DIR}/build/.libs/libSDL2.dylib"
				"${SDL_ROOT_DIR}/build/.libs/libSDL2main.a"
		)
	endif()
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
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		target_link_libraries(${TARGET_NAME} "${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2.lib")
		target_link_libraries(${TARGET_NAME} "${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2main.lib")
	elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
		target_link_libraries(${TARGET_NAME} "${SDL_ROOT_DIR}/build/.libs/libSDL2.dylib")
		target_link_libraries(${TARGET_NAME} "${SDL_ROOT_DIR}/build/.libs/libSDL2main.a")
	endif()
endfunction()

function(CopySDLFiles TARGET_NAME EXECUTABLE_SUBDIR)
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		configure_file(
			"${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2.dll"
			"${PROJECT_BINARY_DIR}/${EXECUTABLE_SUBDIR}/${CE_CONFIGURATION}/SDL2.dll"
			COPYONLY
		)
	elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
		add_custom_command(
			TARGET ${TARGET_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
				"${SDL_ROOT_DIR}/build/.libs/libSDL2.dylib"
				"${PROJECT_BINARY_DIR}/engine/${CE_CONFIGURATION}/CompositeEngine.app/Contents/Frameworks/libSDL2.dylib"
			VERBATIM
		)
	endif()
endfunction()
