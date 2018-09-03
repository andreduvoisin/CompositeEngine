cmake_minimum_required(VERSION 3.7.2)

add_executable(AssetConverter ${ASSET_CONVERTER_SRC_FILES}
	"${ENGINE_SRC_DIR}/common/Math.cpp"
	"${ENGINE_SRC_DIR}/graphics/ceasset/AssetTraits.cpp"
	"${ENGINE_SRC_DIR}/graphics/ceasset/output/AssetExporter.cpp"
	"${ENGINE_SRC_DIR}/graphics/ceasset/output/AssetSerializer.cpp"
	"${ENGINE_SRC_DIR}/graphics/ceasset/output/OutputFileStream.cpp")

include_directories(${ASSET_CONVERTER_SRC_DIR})
include_directories(${ENGINE_SRC_DIR})

if(WIN32)
	target_compile_options(AssetConverter PRIVATE /W3 /WX)
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
	target_link_libraries(AssetConverter "${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk-md.lib")

	# Dynamically-Linked Library
	#target_compile_definitions(AssetConverter PRIVATE FBXSDK_SHARED)
	#target_link_libraries(AssetConverter "${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk.lib")
	#configure_file(
	#	"${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk.dll"
	#	"${PROJECT_BINARY_DIR}/asset-converter/${CE_CONFIGURATION}/libfbxsdk.dll"
	#	COPYONLY
	#)
endif()
