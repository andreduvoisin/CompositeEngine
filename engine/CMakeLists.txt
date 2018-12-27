include("${CMAKE_DIR}/OpenGL.cmake")
include("${CMAKE_DIR}/SDL.cmake")
include("${CMAKE_DIR}/GLEW.cmake")
include("${CMAKE_DIR}/GLM.cmake")
include("${CMAKE_DIR}/CEF.cmake")
include("${CMAKE_DIR}/RapidJSON.cmake")

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
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
		GLEW
		SDL
)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${ENGINE_SRC_FILES})

include_directories(${ENGINE_SRC_DIR})
include_directories(${UI_SRC_DIR})

# TODO: ONLY FOR DEBUG BUILDS
target_compile_options(CompositeEngine PRIVATE -g)

if(WIN32)
	target_compile_options(CompositeEngine PRIVATE /W3 /WX)
endif()

BootstrapOpenGL(CompositeEngine)
BootstrapSDL(CompositeEngine "engine")
BootstrapGLEW(CompositeEngine "engine")
BootstrapGLM()
BootstrapCEF(CompositeEngine "engine")
BootstrapRapidJSON()
