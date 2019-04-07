
add_executable(CompositeEngine MACOSX_BUNDLE ${ENGINE_SRC_FILES})

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${ENGINE_SRC_FILES})

add_dependencies(CompositeEngine CompositeCefSubprocess)
target_link_libraries(CompositeEngine PRIVATE CEF GLEW GLM OpenGL RapidJSON SDL)
target_include_directories(CompositeEngine PRIVATE ${ENGINE_SRC_DIR} ${UI_SRC_DIR})

install(
	TARGETS CompositeEngine
	RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}"
	BUNDLE DESTINATION "${CMAKE_INSTALL_PREFIX}"
)

if(OS_WINDOWS)
	target_compile_options(CompositeEngine PRIVATE /W3 /WX)
	
	# TODO: ONLY FOR DEBUG BUILDS
	install(
		FILES $<TARGET_PDB_FILE:CompositeEngine>
		DESTINATION "${CMAKE_INSTALL_PREFIX}"
		OPTIONAL
	)
	
	install(
		DIRECTORY "${CMAKE_SOURCE_DIR}/engine/graphics/shaders"
		DESTINATION "${CMAKE_INSTALL_PREFIX}"
		FILES_MATCHING
			PATTERN "*.frag"
			PATTERN "*.vert"
	)
	# TODO: This would be much better as a symlink, which CMake supports, but I can't get it to work.
	install(
		DIRECTORY "${CMAKE_SOURCE_DIR}/assets"
		DESTINATION "${CMAKE_INSTALL_PREFIX}"
	)
endif()

if(OS_MACOSX)
	# TODO: ONLY FOR DEBUG BUILDS
	target_compile_options(CompositeEngine PRIVATE -g)

	find_library(CORE_FOUNDATION_LIBRARY CoreFoundation)
	target_link_libraries(CompositeEngine PRIVATE ${CORE_FOUNDATION_LIBRARY})

	set_target_properties(CompositeEngine
		PROPERTIES
			MACOSX_BUNDLE_GUI_IDENTIFIER
			"com.composite.engine")
	
	install(
		DIRECTORY "${CMAKE_SOURCE_DIR}/engine/graphics/shaders"
		DESTINATION "${CMAKE_INSTALL_PREFIX}/CompositeEngine.app/Contents/Resources"
		FILES_MATCHING
			PATTERN "*.frag"
			PATTERN "*.vert"
	)
	# TODO: This would be much better as a symlink, which CMake supports, but I can't get it to work.
	install(
		DIRECTORY "${CMAKE_SOURCE_DIR}/assets"
		DESTINATION "${CMAKE_INSTALL_PREFIX}/CompositeEngine.app/Contents/Resources"
	)
endif()
