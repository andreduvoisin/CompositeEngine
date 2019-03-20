
if(${CMAKE_GENERATOR} STREQUAL "Ninja" OR ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles")
	# By default Ninja and Make builds don't create a subdirectory named after
	# the configuration.
	set(CE_TARGET_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

	# Output binaries (executables, libraries) to the correct directory.
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CE_TARGET_OUT_DIR})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CE_TARGET_OUT_DIR})
else()
	set(CE_TARGET_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")
endif()

add_executable(CompositeAssetConverter ${ASSET_CONVERTER_SRC_FILES})

target_link_libraries(CompositeAssetConverter PRIVATE FBXSDK GLM STB)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${ASSET_CONVERTER_SRC_FILES})

target_include_directories(CompositeAssetConverter PRIVATE ${ASSET_CONVERTER_SRC_DIR})
target_include_directories(CompositeAssetConverter PRIVATE ${ENGINE_SRC_DIR})

if(WIN32)
	target_compile_options(CompositeAssetConverter PRIVATE /W3 /WX)
endif()

install(
	TARGETS CompositeAssetConverter
	RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}"
)
install(
	FILES $<TARGET_PDB_FILE:CompositeAssetConverter>
	DESTINATION "${CMAKE_INSTALL_PREFIX}"
	OPTIONAL
)
