
add_executable(CompositeAssetConverter ${ASSET_CONVERTER_SRC_FILES})

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${ASSET_CONVERTER_SRC_FILES})

target_link_libraries(CompositeAssetConverter PRIVATE FBXSDK GLM STB)
target_include_directories(CompositeAssetConverter PRIVATE ${ASSET_CONVERTER_SRC_DIR} ${ENGINE_SRC_DIR})

install(
	TARGETS CompositeAssetConverter
	RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}"
)

if(OS_WINDOWS)
	target_compile_options(CompositeAssetConverter PRIVATE /W3 /WX)

	# TODO: ONLY FOR DEBUG BUILDS
	install(
		FILES $<TARGET_PDB_FILE:CompositeAssetConverter>
		DESTINATION "${CMAKE_INSTALL_PREFIX}"
		OPTIONAL
	)
endif()
