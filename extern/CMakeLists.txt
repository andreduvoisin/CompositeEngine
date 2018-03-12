cmake_minimum_required(VERSION 3.7.2)

# SDL
if(WIN32)
	set(SDL_ROOT_DIR "${EXTERN_DIR}/SDL2-2.0.5")
	set(SDL_MSVC_DIR "${SDL_ROOT_DIR}/VisualC")
	
	# TODO: Uses v100. What happens if v100 isn't installed?
	execute_process(
		COMMAND
			MSBuild
			"${SDL_MSVC_DIR}/SDL.sln"
			/p:Configuration=${CE_CONFIGURATION}
			/p:Platform=${CE_PLATFORM}
			/m
	)
endif()

# GLEW
if(WIN32)
	set(GLEW_ROOT_DIR "${EXTERN_DIR}/glew-2.1.0")
	set(GLEW_MSVC_DIR "${GLEW_ROOT_DIR}/build/vc12")

	# TODO: Uses v120. What happens if v120 isn't installed?
	execute_process(
		COMMAND
			MSBuild
			"${GLEW_MSVC_DIR}/glew.sln"
			/p:Configuration=${CE_CONFIGURATION}
			/p:Platform=${CE_PLATFORM}
			/m
	)
endif()
