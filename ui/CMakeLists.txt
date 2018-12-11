cmake_minimum_required(VERSION 3.7.2)

set(UI_CEF_SUBPROCESS_SRC_DIR "${UI_SRC_DIR}/cef/subprocess")
file(GLOB_RECURSE UI_CEF_SUBPROCESS_SRC_FILES "${UI_CEF_SUBPROCESS_SRC_DIR}/*.cpp" "${ENGINE_SRC_DIR}/*.manifest")

add_executable(CefSubprocess ${UI_CEF_SUBPROCESS_SRC_FILES})

include_directories(${UI_CEF_SUBPROCESS_SRC_DIR})

if(WIN32)
	target_compile_options(CefSubprocess PRIVATE /W3 /WX)
endif()

# CEF
if(WIN32)
	set(CEF_VERSION_DIR_NAME "cef_binary_3.3497.1840.gcd24143")
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

	target_link_libraries(CefSubprocess "${PROJECT_BINARY_DIR}/extern/${CEF_VERSION_DIR_NAME}/${CEF_PLATFORM_DIR_NAME}/libcef_dll_wrapper/${CEF_CONFIGURATION}/libcef_dll_wrapper.lib")
	target_link_libraries(CefSubprocess "${CEF_PLATFORM_DIR}/${CEF_CONFIGURATION}/libcef.lib")
	#target_link_libraries(CefSubprocess "${CEF_PLATFORM_DIR}/${CEF_CONFIGURATION}/cef_sandbox.lib")

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
