cmake_minimum_required(VERSION 3.7.2)

add_executable(CompositeEngine ${ENGINE_SRC_FILES})

include_directories(${ENGINE_SRC_DIR})

if(WIN32)
	target_compile_options(CompositeEngine PRIVATE /W3 /WX)
endif()

# OpenGL
if(WIN32)
	target_link_libraries(CompositeEngine opengl32) # glu32 gdi32 winmm user32 glut32
endif()

# SDL
if(WIN32)
	set(SDL_ROOT_DIR "${EXTERN_DIR}/SDL2-2.0.8")
	set(SDL_MSVC_DIR "${SDL_ROOT_DIR}/VisualC")

	include_directories("${SDL_ROOT_DIR}/include")
	
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

	target_link_libraries(CompositeEngine "${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2.lib")
	target_link_libraries(CompositeEngine "${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2main.lib")

	configure_file(
		"${SDL_MSVC_DIR}/${SDL_PLATFORM}/${SDL_CONFIGURATION}/SDL2.dll"
		"${PROJECT_BINARY_DIR}/engine/${CE_CONFIGURATION}/SDL2.dll"
		COPYONLY
	)
endif()

# GLEW
if(WIN32)
	set(GLEW_ROOT_DIR "${EXTERN_DIR}/glew-2.1.0")
	
	if(${CE_CONFIGURATION} STREQUAL "Debug")
		set(GLEW_CONFIGURATION "Debug")
		set(GLEW_LIB_CONFIGURATION "d")
	elseif(${CE_CONFIGURATION} STREQUAL "Release")
		set(GLEW_CONFIGURATION "Release")
		set(GLEW_LIB_CONFIGURATION "")
	endif()

	if(${CE_PLATFORM} STREQUAL "Win32")
		set(GLEW_PLATFORM "Win32")
	elseif(${CE_PLATFORM} STREQUAL "x64")
		set(GLEW_PLATFORM "x64")
	endif()

	include_directories("${GLEW_ROOT_DIR}/include")

	# Statically-Linked Library
	target_compile_definitions(CompositeEngine PRIVATE GLEW_STATIC)
	target_link_libraries(CompositeEngine "${GLEW_ROOT_DIR}/lib/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32s${GLEW_LIB_CONFIGURATION}.lib")

	# Dynamically-Linked Library
	#target_link_libraries(CompositeEngine "${GLEW_ROOT_DIR}/lib/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32${GLEW_LIB_CONFIGURATION}.lib")
	#configure_file(
	#	"${GLEW_ROOT_DIR}/bin/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32${GLEW_LIB_CONFIGURATION}.dll"
	#	"${PROJECT_BINARY_DIR}/engine/${CE_CONFIGURATION}/glew32${GLEW_LIB_CONFIGURATION}.dll"
	#	COPYONLY
	#)
endif()

# GLM
include_directories("${EXTERN_DIR}/glm-0.9.8.5")

# CEF
if(WIN32)
	set(CEF_VERSION_DIR_NAME "cef_binary_3.3440.1805.gbe070f9")
	set(CEF_PLATFORM_DIR_NAME "windows64")

	set(CEF_ROOT_DIR "${EXTERN_DIR}/${CEF_VERSION_DIR_NAME}")
	set(CEF_PLATFORM_DIR "${CEF_ROOT_DIR}/${CEF_PLATFORM_DIR_NAME}")

	include_directories("${CEF_PLATFORM_DIR}")
	
	if(${CE_CONFIGURATION} STREQUAL "Debug")
		set(CEF_CONFIGURATION "Debug")
	elseif(${CE_CONFIGURATION} STREQUAL "Release")
		set(CEF_CONFIGURATION "Release")
	endif()

	if(${CE_PLATFORM} STREQUAL "Win32")
		set(CEF_PLATFORM "Win32")
	elseif(${CE_PLATFORM} STREQUAL "x64")
		set(CEF_PLATFORM "x64")
	endif()

	target_link_libraries(CompositeEngine "${PROJECT_BINARY_DIR}/extern/${CEF_VERSION_DIR_NAME}/${CEF_PLATFORM_DIR_NAME}/libcef_dll_wrapper/${CEF_CONFIGURATION}/libcef_dll_wrapper.lib")
	target_link_libraries(CompositeEngine "${CEF_PLATFORM_DIR}/${CEF_CONFIGURATION}/libcef.lib")
	#target_link_libraries(CompositeEngine "${CEF_PLATFORM_DIR}/${CEF_CONFIGURATION}/cef_sandbox.lib")

	file(GLOB CEF_BINARY_FILES "${CEF_PLATFORM_DIR}/${CEF_CONFIGURATION}/*")
	file(
		COPY ${CEF_BINARY_FILES}
		DESTINATION "${PROJECT_BINARY_DIR}/engine/${CE_CONFIGURATION}"
		FILES_MATCHING
			PATTERN "*.dll"
			PATTERN "*.bin"
	)

	file(GLOB CEF_RESOURCE_FILES "${CEF_PLATFORM_DIR}/Resources/*")
	file(
		COPY ${CEF_RESOURCE_FILES}
		DESTINATION "${PROJECT_BINARY_DIR}/engine/${CE_CONFIGURATION}"
	)
endif()

if(WIN32)
	target_link_libraries(CompositeEngine ws2_32)
endif()
