cmake_minimum_required(VERSION 3.13.0)

project(CompositeEngine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_SIZEOF_VOID_P MATCHES 8)
	message(FATAL_ERROR "Composite Engine only supports 64-bit builds.")
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	set(OS_WINDOWS 1)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
	set(OS_MACOSX 1)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Reference: https://gitlab.kitware.com/cmake/community/wikis/FAQ#how-can-i-build-my-msvc-application-with-a-static-runtime
function(RemoveCMakeCXXFlag flag)
	foreach(flags_variable
			CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
			CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		if(${flags_variable} MATCHES ${flag})
			string(REGEX REPLACE ${flag} "" ${flags_variable} "${${flags_variable}}")
			set(${flags_variable} "${${flags_variable}}" PARENT_SCOPE)
		endif()
	endforeach()
endfunction()

# Reference: https://gitlab.kitware.com/cmake/cmake/issues/19084
if(OS_WINDOWS)
	# set warning level and turn on warnings as errors
	RemoveCMakeCXXFlag("/W3")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")

	# disable RTTI
	RemoveCMakeCXXFlag("/GR")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")

	# disable exceptions
	RemoveCMakeCXXFlag("/EHsc")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHs-c- /D_HAS_EXCEPTIONS=0")
endif()

set(CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(EXTERN_DIR "${CMAKE_CURRENT_LIST_DIR}/extern")
set(ENGINE_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/engine")
set(UI_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/ui")
set(CEF_SUBPROCESS_SRC_DIR "${UI_SRC_DIR}/cef/subprocess")
set(ASSET_CONVERTER_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/asset-converter")

# TODO: Move CefMain.cpp and WindowContentView.mm to better locations.
file(GLOB_RECURSE ENGINE_SRC_FILES
	"${ENGINE_SRC_DIR}/*.cpp"
	"${UI_SRC_DIR}/cef/browser/*.cpp"
	"${UI_SRC_DIR}/cef/client/*.cpp"
	"${UI_SRC_DIR}/cef/CefMain.cpp")
if(OS_WINDOWS)
	set(ENGINE_SRC_FILES ${ENGINE_SRC_FILES} "${ENGINE_SRC_DIR}/CompositeEngine.manifest")
endif()
if(OS_MACOSX)
	file(GLOB_RECURSE ENGINE_SRC_FILES_MACOSX
		"${UI_SRC_DIR}/cef/WindowContentView.mm")
	set(ENGINE_SRC_FILES ${ENGINE_SRC_FILES} ${ENGINE_SRC_FILES_MACOSX})
endif()

file(GLOB_RECURSE CEF_SUBPROCESS_SRC_FILES
	"${CEF_SUBPROCESS_SRC_DIR}/*.cpp")
if(OS_WINDOWS)
	set(CEF_SUBPROCESS_SRC_FILES ${CEF_SUBPROCESS_SRC_FILES} "${UI_SRC_DIR}/CompositeCefSubprocess.manifest")
endif()

file(GLOB_RECURSE ASSET_CONVERTER_SRC_FILES
	"${ASSET_CONVERTER_SRC_DIR}/*.cpp"
	"${ENGINE_SRC_DIR}/common/Math.cpp"
	"${ENGINE_SRC_DIR}/graphics/ceasset/AssetTraits.cpp"
	"${ENGINE_SRC_DIR}/graphics/ceasset/output/AssetExporter.cpp"
	"${ENGINE_SRC_DIR}/graphics/ceasset/output/AssetSerializer.cpp"
	"${ENGINE_SRC_DIR}/graphics/ceasset/output/OutputFileStream.cpp")

add_subdirectory(${EXTERN_DIR})
add_subdirectory(${ENGINE_SRC_DIR})
add_subdirectory(${UI_SRC_DIR})
add_subdirectory(${ASSET_CONVERTER_SRC_DIR})
