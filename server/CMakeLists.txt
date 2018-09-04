cmake_minimum_required(VERSION 3.7.2)

add_executable(CompositeServer ${SERVER_SRC_FILES})

include_directories(${SERVER_SRC_DIR})

if(WIN32)
    target_compile_options(CompositeServer PRIVATE /W3 /WX)
endif()

if(WIN32)
	target_link_libraries(CompositeServer ws2_32)
endif()
