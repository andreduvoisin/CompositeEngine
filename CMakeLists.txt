cmake_minimum_required(VERSION 3.7.2)

project(CompositeEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(EXTERN_DIR "${CMAKE_CURRENT_LIST_DIR}/extern")
set(ENGINE_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/engine")
set(UI_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/ui")
set(CEF_SUBPROCESS_SRC_DIR "${UI_SRC_DIR}/cef/subprocess")
set(ASSET_CONVERTER_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/asset-converter")

file(GLOB_RECURSE ENGINE_SRC_FILES
	"${ENGINE_SRC_DIR}/*.cpp"
	"${UI_SRC_DIR}/cef/browser/*.cpp"
	"${UI_SRC_DIR}/cef/client/*.cpp"
	"${ENGINE_SRC_DIR}/*.manifest")
file(GLOB_RECURSE CEF_SUBPROCESS_SRC_FILES
	"${CEF_SUBPROCESS_SRC_DIR}/*.cpp"
	"${ENGINE_SRC_DIR}/*.manifest")
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
