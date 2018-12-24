include("${CMAKE_DIR}/CEF.cmake")

add_executable(CompositeCefSubprocess ${CEF_SUBPROCESS_SRC_FILES})

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${CEF_SUBPROCESS_SRC_FILES})

include_directories(${CEF_SUBPROCESS_SRC_DIR})

if(WIN32)
	target_compile_options(CompositeCefSubprocess PRIVATE /W3 /WX)
endif()

BootstrapCEF(CompositeCefSubprocess "ui")
