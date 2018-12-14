include("${CMAKE_DIR}/GLM.cmake")
include("${CMAKE_DIR}/STBImage.cmake")
include("${CMAKE_DIR}/FBXSDK.cmake")

add_executable(CompositeAssetConverter ${ASSET_CONVERTER_SRC_FILES}
	"${ENGINE_SRC_DIR}/common/Math.cpp"
	"${ENGINE_SRC_DIR}/graphics/ceasset/AssetTraits.cpp"
	"${ENGINE_SRC_DIR}/graphics/ceasset/output/AssetExporter.cpp"
	"${ENGINE_SRC_DIR}/graphics/ceasset/output/AssetSerializer.cpp"
	"${ENGINE_SRC_DIR}/graphics/ceasset/output/OutputFileStream.cpp")

include_directories(${ASSET_CONVERTER_SRC_DIR})
include_directories(${ENGINE_SRC_DIR})

if(WIN32)
	target_compile_options(CompositeAssetConverter PRIVATE /W3 /WX)
endif()

BootstrapGLM()
BootstrapSTBImage()
BootstrapFBXSDK(CompositeAssetConverter "asset-converter")
