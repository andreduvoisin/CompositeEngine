
add_executable(CompositeServer ${SERVER_SRC_FILES})

include_directories(${SERVER_SRC_DIR})

if(OS_WINDOWS)
    target_compile_options(CompositeServer PRIVATE /W3 /WX)

	target_link_libraries(CompositeServer PRIVATE ws2_32)
endif()
