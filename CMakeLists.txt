cmake_minimum_required(VERSION 3.7.2)

project(CompositeEngine)

set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(EXTERN_DIR "${CMAKE_CURRENT_LIST_DIR}/extern")

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")

add_executable(CompositeEngine ${SRC_FILES})

include_directories(${SRC_DIR})

# OpenGL
if(WIN32)
	target_link_libraries(CompositeEngine opengl32) # glu32 gdi32 winmm user32 glut32
endif()

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

	include_directories("${SDL_ROOT_DIR}/include")

	target_link_libraries(CompositeEngine "${SDL_MSVC_DIR}/${CE_PLATFORM}/${CE_CONFIGURATION}/SDL2.lib")
	target_link_libraries(CompositeEngine "${SDL_MSVC_DIR}/${CE_PLATFORM}/${CE_CONFIGURATION}/SDL2main.lib")

	configure_file(
		"${SDL_MSVC_DIR}/${CE_PLATFORM}/${CE_CONFIGURATION}/SDL2.dll"
		"${PROJECT_BINARY_DIR}/${CE_CONFIGURATION}/SDL2.dll"
		COPYONLY
	)
endif()

# GLEW
if(WIN32)
	set(GLEW_ROOT_DIR "${EXTERN_DIR}/glew-2.1.0")
	set(GLEW_MSVC_DIR "${GLEW_ROOT_DIR}/build/vc12")

	if(${CE_CONFIGURATION} STREQUAL "Debug")
		set(GLEW_CONFIGURATION "d")
	else()
		set(GLEW_CONFIGURATION "")
	endif()
	
	# TODO: Uses v120. What happens if v120 isn't installed?
	execute_process(
		COMMAND
			MSBuild
			"${GLEW_MSVC_DIR}/glew.sln"
			/p:Configuration=${CE_CONFIGURATION}
			/p:Platform=${CE_PLATFORM}
			/m
	)

	include_directories("${GLEW_ROOT_DIR}/include")

	# Statically-Linked Library
	target_compile_definitions(CompositeEngine PRIVATE GLEW_STATIC)
	target_link_libraries(CompositeEngine "${GLEW_ROOT_DIR}/lib/${CE_CONFIGURATION}/${CE_PLATFORM}/glew32s${GLEW_CONFIGURATION}.lib")

	# Dynamically-Linked Library
	#target_link_libraries(CompositeEngine "${GLEW_ROOT_DIR}/lib/${CE_CONFIGURATION}/${CE_PLATFORM}/glew32${GLEW_CONFIGURATION}.lib")
	#configure_file(
	#	"${GLEW_ROOT_DIR}/bin/${CE_CONFIGURATION}/${CE_PLATFORM}/glew32${GLEW_CONFIGURATION}.dll"
	#	"${PROJECT_BINARY_DIR}/${CE_CONFIGURATION}/glew32${GLEW_CONFIGURATION}.dll"
	#	COPYONLY
	#)
endif()

# GLM
include_directories("${EXTERN_DIR}/glm-0.9.8.5")

# stb_image
include_directories("${EXTERN_DIR}/stb_image-2.16")

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

	# FBX SDK can be linked statically (with either /MD or /MT) or dynamically.
	# opengl32.lib (and glu32.lib) link with /MD, so we can't link with /MT.
	# /MD[d] is included in CMAKE_CXX_FLAGS[_*] by default.

	# Statically-Linked Library (/MD)
	target_link_libraries(CompositeEngine "${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk-md.lib")

	# Dynamically-Linked Library
	#target_compile_definitions(CompositeEngine PRIVATE FBXSDK_SHARED)
	#target_link_libraries(CompositeEngine "${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk.lib")
	#configure_file(
	#	"${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk.dll"
	#	"${PROJECT_BINARY_DIR}/${CE_CONFIGURATION}/libfbxsdk.dll"
	#	COPYONLY
	#)
endif()
