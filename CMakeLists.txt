cmake_minimum_required(VERSION 3.7.2)

project(CompositeEngine)

set(EXTERN_DIR "${CMAKE_CURRENT_LIST_DIR}/extern")
set(ENGINE_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/engine")
set(ASSET_CONVERTER_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/asset-converter")

file(GLOB_RECURSE ENGINE_SRC_FILES "${ENGINE_SRC_DIR}/*.cpp")
file(GLOB_RECURSE ASSET_CONVERTER_SRC_FILES "${ASSET_CONVERTER_SRC_DIR}/*.cpp")

add_subdirectory(${EXTERN_DIR})
add_subdirectory(${ENGINE_SRC_DIR})
add_subdirectory(${ASSET_CONVERTER_SRC_DIR})

add_executable(CompositeServer ${ENGINE_SRC_DIR}/server/ServerMain.cpp)
if(WIN32)
  target_link_libraries(CompositeServer wsock32 ws2_32)
endif()
