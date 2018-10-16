cmake_minimum_required(VERSION 3.7.2)

# SDL
if(WIN32)
	set(SDL_ROOT_DIR "${EXTERN_DIR}/SDL2-2.0.8")
	set(SDL_MSVC_DIR "${SDL_ROOT_DIR}/VisualC")
	
	# TODO: Uses v100. What happens if v100 isn't installed?
	execute_process(
		COMMAND
			MSBuild
			"${SDL_MSVC_DIR}/SDL.sln"
			/p:PlatformToolset=v141
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
			/p:PlatformToolset=v141
			/p:Configuration=${CE_CONFIGURATION}
			/p:Platform=${CE_PLATFORM}
			/m
	)
endif()

# CEF
if(WIN32)
	set(CEF_ROOT_DIR "${EXTERN_DIR}/cef_binary_3.3497.1840.gcd24143")
	set(CEF_PLATFORM_DIR "${CEF_ROOT_DIR}/windows64")
	
	set(CEF_RUNTIME_LIBRARY_FLAG "/MD")

	add_subdirectory(${CEF_PLATFORM_DIR})
endif()
