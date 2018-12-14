include("${CMAKE_DIR}/CEF.cmake")

set(UI_CEF_SUBPROCESS_SRC_DIR "${UI_SRC_DIR}/cef/subprocess")
file(GLOB_RECURSE UI_CEF_SUBPROCESS_SRC_FILES "${UI_CEF_SUBPROCESS_SRC_DIR}/*.cpp" "${ENGINE_SRC_DIR}/*.manifest")

add_executable(CompositeCefSubprocess ${UI_CEF_SUBPROCESS_SRC_FILES})

include_directories(${UI_CEF_SUBPROCESS_SRC_DIR})

if(WIN32)
	target_compile_options(CompositeCefSubprocess PRIVATE /W3 /WX)
endif()

BootstrapCEF(CompositeCefSubprocess "ui")
