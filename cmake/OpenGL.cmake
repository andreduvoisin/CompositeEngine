
add_library(OpenGL INTERFACE)

 # glu32 gdi32 winmm user32 glut32
if(OS_WINDOWS)
	target_link_libraries(OpenGL INTERFACE opengl32)
elseif(OS_MACOSX)
	find_library(OPENGL_LIBRARY OpenGL)
	target_link_libraries(OpenGL INTERFACE ${OPENGL_LIBRARY})
endif()
