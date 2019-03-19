
function(BootstrapOpenGL TARGET_NAME)
	LinkOpenGL(${TARGET_NAME})
endfunction()

function(LinkOpenGL TARGET_NAME)
	if(OS_WINDOWS)
		set(OPENGL_LIBRARY opengl32)
	elseif(OS_MACOSX)
		find_library(OPENGL_LIBRARY OpenGL)
	endif()

	target_link_libraries(${TARGET_NAME} ${OPENGL_LIBRARY}) # glu32 gdi32 winmm user32 glut32
endfunction()
