function(BootstrapOpenGL TARGET_NAME)
	LinkOpenGL(${TARGET_NAME})
endfunction()

function(LinkOpenGL TARGET_NAME)
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		target_link_libraries(${TARGET_NAME} opengl32) # glu32 gdi32 winmm user32 glut32
	elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
		find_library(OPENGL_LIBRARY OpenGL)
		target_link_libraries(${TARGET_NAME} ${OPENGL_LIBRARY})
	endif()
endfunction()
