
add_library(OpenGL INTERFACE)

# glu32 gdi32 winmm user32 glut32
if(OS_WINDOWS)
	set(OPENGL_LIBRARY opengl32)
endif()

if(OS_MACOSX)
	find_library(OPENGL_LIBRARY OpenGL)
endif()

target_link_libraries(OpenGL INTERFACE ${OPENGL_LIBRARY})
