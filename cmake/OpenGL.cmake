function(BootstrapOpenGL TARGET_NAME)
	LinkOpenGL(${TARGET_NAME})
endfunction(BootstrapOpenGL)

function(LinkOpenGL TARGET_NAME)
	target_link_libraries(${TARGET_NAME} opengl32) # glu32 gdi32 winmm user32 glut32
endfunction(LinkOpenGL)
