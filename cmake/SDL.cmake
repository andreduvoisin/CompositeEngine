set(SDL_ROOT_DIR "${EXTERN_DIR}/SDL2-2.0.8")
set(SDL_MSVC_DIR "${SDL_ROOT_DIR}/VisualC")

if(${CE_CONFIGURATION} STREQUAL "Debug")
	set(SDL_CONFIGURATION "Debug")
elseif(${CE_CONFIGURATION} STREQUAL "Release")
	set(SDL_CONFIGURATION "Release")
endif()

if(${CE_PLATFORM} STREQUAL "Win32")
	set(SDL_PLATFORM "Win32")
elseif(${CE_PLATFORM} STREQUAL "x64")
	set(SDL_PLATFORM "x64")
endif()

function(BuildSDL)
	execute_process(
		COMMAND
			MSBuild
			"${SDL_MSVC_DIR}/SDL.sln"
			/p:PlatformToolset=v141 # Default: v100
			/p:Configuration=${CE_CONFIGURATION}
			/p:Platform=${CE_PLATFORM}
			/m
	)
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
	target_link_libraries(${TARGET_NAME} "${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2.lib")
	target_link_libraries(${TARGET_NAME} "${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2main.lib")
endfunction(LinkSDL)

function(CopySDLFiles EXECUTABLE_SUBDIR)
	configure_file(
		"${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2.dll"
		"${PROJECT_BINARY_DIR}/${EXECUTABLE_SUBDIR}/${CE_CONFIGURATION}/SDL2.dll"
		COPYONLY
	)
endfunction(CopySDLFiles)
