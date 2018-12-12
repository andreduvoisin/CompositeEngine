include("${CMAKE_DIR}/OpenGL.cmake")
include("${CMAKE_DIR}/SDL.cmake")
include("${CMAKE_DIR}/GLEW.cmake")
include("${CMAKE_DIR}/GLM.cmake")
include("${CMAKE_DIR}/CEF.cmake")
include("${CMAKE_DIR}/RapidJSON.cmake")

add_executable(CompositeEngine ${ENGINE_SRC_FILES})

include_directories(${ENGINE_SRC_DIR})
include_directories(${UI_SRC_DIR})

if(WIN32)
	target_compile_options(CompositeEngine PRIVATE /W3 /WX)
endif()

BootstrapOpenGL(CompositeEngine)
BootstrapSDL(CompositeEngine "engine")
BootstrapGLEW(CompositeEngine "engine")
BootstrapGLM()
BootstrapCEF(CompositeEngine "engine")
BootstrapRapidJSON()
