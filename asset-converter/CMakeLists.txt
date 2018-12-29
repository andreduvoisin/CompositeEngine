include("${CMAKE_DIR}/GLM.cmake")
include("${CMAKE_DIR}/STB.cmake")
include("${CMAKE_DIR}/FBXSDK.cmake")

add_executable(CompositeAssetConverter ${ASSET_CONVERTER_SRC_FILES})

add_dependencies(
	CompositeAssetConverter
		STB
)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${ASSET_CONVERTER_SRC_FILES})

include_directories(${ASSET_CONVERTER_SRC_DIR})
include_directories(${ENGINE_SRC_DIR})

if(WIN32)
	target_compile_options(CompositeAssetConverter PRIVATE /W3 /WX)
endif()

BootstrapGLM()
BootstrapSTB()
BootstrapFBXSDK(CompositeAssetConverter "asset-converter")
