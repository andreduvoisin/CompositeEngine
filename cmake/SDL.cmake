set(SDL_ROOT_DIR "${EXTERN_DIR}/SDL2-2.0.8")
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
		execute_process(COMMAND ./configure WORKING_DIRECTORY "${SDL_ROOT_DIR}")
		execute_process(COMMAND make WORKING_DIRECTORY "${SDL_ROOT_DIR}")
		# TODO: Should this be installed, or should we just point to the local dylib?
		# https://developer.apple.com/library/archive/documentation/DeveloperTools/Conceptual/DynamicLibraries/100-Articles/UsingDynamicLibraries.html
		execute_process(COMMAND sudo make install WORKING_DIRECTORY "${SDL_ROOT_DIR}")
	endif()
endfunction(BuildSDL)

function(BootstrapSDL TARGET_NAME EXECUTABLE_SUBDIR)
	IncludeSDL()
	LinkSDL(${TARGET_NAME})
	CopySDLFiles(${EXECUTABLE_SUBDIR})
endfunction(BootstrapSDL)

function(IncludeSDL)
	include_directories("${SDL_ROOT_DIR}/include")
endfunction(IncludeSDL)

function(LinkSDL TARGET_NAME)
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		target_link_libraries(${TARGET_NAME} "${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2.lib")
		target_link_libraries(${TARGET_NAME} "${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2main.lib")
	elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
		target_link_libraries(${TARGET_NAME} "${SDL_ROOT_DIR}/build/.libs/libSDL2.dylib")
		target_link_libraries(${TARGET_NAME} "${SDL_ROOT_DIR}/build/.libs/libSDL2main.a")
	endif()
endfunction(LinkSDL)

function(CopySDLFiles EXECUTABLE_SUBDIR)
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		configure_file(
			"${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2.dll"
			"${PROJECT_BINARY_DIR}/${EXECUTABLE_SUBDIR}/${CE_CONFIGURATION}/SDL2.dll"
			COPYONLY
		)
	endif()
endfunction(CopySDLFiles)
