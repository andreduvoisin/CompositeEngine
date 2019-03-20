include("${CMAKE_DIR}/CEF.cmake")

if(OS_MACOSX)
	set(COMPOSITE_ENGINE_EXECUTABLE_ARGS MACOSX_BUNDLE)
endif()

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

add_executable(CompositeEngine ${COMPOSITE_ENGINE_EXECUTABLE_ARGS} ${ENGINE_SRC_FILES})

add_dependencies(
	CompositeEngine
		CompositeCefSubprocess
		CEF
		#GLEW
		#GLM
		#RapidJSON
		#SDL
)

target_link_libraries(CompositeEngine PRIVATE GLEW GLM OpenGL RapidJSON SDL)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${ENGINE_SRC_FILES})

include_directories(${ENGINE_SRC_DIR})
include_directories(${UI_SRC_DIR})

# TODO: ONLY FOR DEBUG BUILDS (AND NOT ON WINDOWS?)
target_compile_options(CompositeEngine PRIVATE -g)

if(WIN32)
	target_compile_options(CompositeEngine PRIVATE /W3 /WX)
	#target_link_libraries(CompositeEngine -manifest:embed -manifestinput:"${ENGINE_SRC_DIR}/CompositeEngine.manifest")
endif()

BootstrapCEF(CompositeEngine "engine")

install(
	TARGETS CompositeEngine
	RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}"
)
install(
	FILES $<TARGET_PDB_FILE:CompositeEngine>
	DESTINATION "${CMAKE_INSTALL_PREFIX}"
	OPTIONAL
)
