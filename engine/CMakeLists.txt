cmake_minimum_required(VERSION 3.7.2)

project(Engine)

add_executable(Engine ${ENGINE_SRC_FILES})

include_directories(${ENGINE_SRC_DIR})

# OpenGL
if(WIN32)
	target_link_libraries(Engine opengl32) # glu32 gdi32 winmm user32 glut32
endif()

# SDL
if(WIN32)
	set(SDL_ROOT_DIR "${EXTERN_DIR}/SDL2-2.0.5")
	set(SDL_MSVC_DIR "${SDL_ROOT_DIR}/VisualC")

	include_directories("${SDL_ROOT_DIR}/include")

	target_link_libraries(Engine "${SDL_MSVC_DIR}/${CE_PLATFORM}/${CE_CONFIGURATION}/SDL2.lib")
	target_link_libraries(Engine "${SDL_MSVC_DIR}/${CE_PLATFORM}/${CE_CONFIGURATION}/SDL2main.lib")

	configure_file(
		"${SDL_MSVC_DIR}/${CE_PLATFORM}/${CE_CONFIGURATION}/SDL2.dll"
		"${PROJECT_BINARY_DIR}/${CE_CONFIGURATION}/SDL2.dll"
		COPYONLY
	)
endif()

# GLEW
if(WIN32)
	set(GLEW_ROOT_DIR "${EXTERN_DIR}/glew-2.1.0")

	if(${CE_CONFIGURATION} STREQUAL "Debug")
		set(GLEW_CONFIGURATION "d")
	else()
		set(GLEW_CONFIGURATION "")
	endif()

	include_directories("${GLEW_ROOT_DIR}/include")

	# Statically-Linked Library
	target_compile_definitions(Engine PRIVATE GLEW_STATIC)
	target_link_libraries(Engine "${GLEW_ROOT_DIR}/lib/${CE_CONFIGURATION}/${CE_PLATFORM}/glew32s${GLEW_CONFIGURATION}.lib")

	# Dynamically-Linked Library
	#target_link_libraries(Engine "${GLEW_ROOT_DIR}/lib/${CE_CONFIGURATION}/${CE_PLATFORM}/glew32${GLEW_CONFIGURATION}.lib")
	#configure_file(
	#	"${GLEW_ROOT_DIR}/bin/${CE_CONFIGURATION}/${CE_PLATFORM}/glew32${GLEW_CONFIGURATION}.dll"
	#	"${PROJECT_BINARY_DIR}/${CE_CONFIGURATION}/glew32${GLEW_CONFIGURATION}.dll"
	#	COPYONLY
	#)
endif()

# GLM
include_directories("${EXTERN_DIR}/glm-0.9.8.5")
