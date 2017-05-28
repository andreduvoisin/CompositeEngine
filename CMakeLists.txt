cmake_minimum_required(VERSION 3.7.2)

project(CompositeEngine)

set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(EXTERN_DIR "${CMAKE_CURRENT_LIST_DIR}/extern")

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")

# DirectX Shaders
if(WIN32)
	set(PIXEL_SHADERS "${SRC_DIR}/shaders/SimplePixelShader.hlsl")
	set(VERTEX_SHADERS "${SRC_DIR}/shaders/SimpleVertexShader.hlsl")

	list(APPEND SRC_FILES ${PIXEL_SHADERS} ${VERTEX_SHADERS})

	set_source_files_properties(
		${PIXEL_SHADERS}
		PROPERTIES
			VS_SHADER_TYPE Pixel
			VS_SHADER_MODEL 4.0_level_9_3
			VS_SHADER_ENTRYPOINT main
	)

	set_source_files_properties(
		${VERTEX_SHADERS}
		PROPERTIES
			VS_SHADER_TYPE Vertex
			VS_SHADER_MODEL 4.0_level_9_3
			VS_SHADER_ENTRYPOINT main
	)
endif()

add_executable(CompositeEngine ${SRC_FILES})

include_directories(${SRC_DIR})

# freeglut/glew/glm
# TODO: Remove these dependencies.
include_directories(
	"${EXTERN_DIR}/freeglut-2.8.1/include"
	"${EXTERN_DIR}/glew-1.10.0/include"
	"${EXTERN_DIR}/glm-0.9.5.1"
)

target_link_libraries(
	CompositeEngine
		"${EXTERN_DIR}/freeglut-2.8.1/lib/x86/Debug/freeglut_static.lib"
		"${EXTERN_DIR}/glew-1.10.0/lib/Debug/Win32/glew32sd.lib"
)

# DirectX
if(WIN32)
	target_link_libraries(CompositeEngine d3d11 d3dcompiler)
endif()

# SDL
if(WIN32)
	set(SDL_ROOT_DIR "${EXTERN_DIR}/SDL2-2.0.5")
	set(SDL_MSVC_DIR "${SDL_ROOT_DIR}/VisualC")

	execute_process(
		COMMAND
			MSBuild
			"${SDL_MSVC_DIR}/SDL.sln"
			/p:PlatformToolset=v141
			/p:Configuration=${CE_CONFIGURATION}
			/p:Platform=${CE_PLATFORM}
			/m
	)

	include_directories("${SDL_ROOT_DIR}/include")

	target_link_libraries(CompositeEngine "${SDL_MSVC_DIR}/${CE_PLATFORM}/${CE_CONFIGURATION}/SDL2.lib")
	target_link_libraries(CompositeEngine "${SDL_MSVC_DIR}/${CE_PLATFORM}/${CE_CONFIGURATION}/SDL2main.lib")

	configure_file(
		"${SDL_MSVC_DIR}/${CE_PLATFORM}/${CE_CONFIGURATION}/SDL2.dll"
		"${PROJECT_BINARY_DIR}/${CE_CONFIGURATION}/SDL2.dll"
		COPYONLY
	)
endif()

# FBX SDK
if(WIN32)
	set(FBXSDK_ROOT_DIR "${EXTERN_DIR}/FBX SDK/2018.1.1")
	set(FBXSDK_MSVC_DIR "${FBXSDK_ROOT_DIR}/lib/vs2015")
	
	if(${CE_CONFIGURATION} STREQUAL "Debug")
		set(FBXSDK_CONFIGURATION "debug")
	elseif(${CE_CONFIGURATION} STREQUAL "Release")
		set(FBXSDK_CONFIGURATION "release")
	endif()

	if(${CE_PLATFORM} STREQUAL "Win32")
		set(FBXSDK_PLATFORM "x86")
	elseif(${CE_PLATFORM} STREQUAL "x64")
		set(FBXSDK_PLATFORM "x64")
	endif()

	include_directories("${FBXSDK_ROOT_DIR}/include")

	target_link_libraries(CompositeEngine "${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk.lib")

	configure_file(
		"${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk.dll"
		"${PROJECT_BINARY_DIR}/${CE_CONFIGURATION}/libfbxsdk.dll"
		COPYONLY
	)
endif()
