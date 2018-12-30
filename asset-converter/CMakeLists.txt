include("${CMAKE_DIR}/FBXSDK.cmake")
include("${CMAKE_DIR}/GLM.cmake")
include("${CMAKE_DIR}/STB.cmake")

add_executable(CompositeAssetConverter ${ASSET_CONVERTER_SRC_FILES})

add_dependencies(
	CompositeAssetConverter
		FBXSDK
		GLM
		STB
)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${ASSET_CONVERTER_SRC_FILES})

include_directories(${ASSET_CONVERTER_SRC_DIR})
include_directories(${ENGINE_SRC_DIR})

if(WIN32)
	target_compile_options(CompositeAssetConverter PRIVATE /W3 /WX)
endif()

BootstrapFBXSDK(CompositeAssetConverter "asset-converter")
BootstrapGLM()
BootstrapSTB()
