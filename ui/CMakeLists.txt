
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
	set(COMPOSITE_CEF_SUBPROCESS_EXECUTABLE_ARGS MACOSX_BUNDLE)
endif()

# TODO: Is this needed?
if(${CMAKE_GENERATOR} STREQUAL "Ninja" OR ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles")
	# By default Ninja and Make builds don't create a subdirectory named after
	# the configuration.
	set(CE_CEF_SUBPROCESS_TARGET_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

	# Output binaries (executables, libraries) to the correct directory.
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CE_CEF_SUBPROCESS_TARGET_OUT_DIR})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CE_CEF_SUBPROCESS_TARGET_OUT_DIR})
else()
	set(CE_CEF_SUBPROCESS_TARGET_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")
endif()

add_executable(CompositeCefSubprocess ${COMPOSITE_CEF_SUBPROCESS_EXECUTABLE_ARGS} ${CEF_SUBPROCESS_SRC_FILES})

target_link_libraries(CompositeCefSubprocess PRIVATE CEF)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${CEF_SUBPROCESS_SRC_FILES})

include_directories(${CEF_SUBPROCESS_SRC_DIR})

if(WIN32)
	target_compile_options(CompositeCefSubprocess PRIVATE /W3 /WX)
	#target_link_libraries(CompositeCefSubprocess -manifest:embed -manifestinput:"${ENGINE_SRC_DIR}/CompositeEngine.manifest")
endif()

install(
	TARGETS CompositeCefSubprocess
	RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}"
)
install(
	FILES $<TARGET_PDB_FILE:CompositeCefSubprocess>
	DESTINATION "${CMAKE_INSTALL_PREFIX}"
	OPTIONAL
)
