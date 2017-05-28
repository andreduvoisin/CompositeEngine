cmake_minimum_required(VERSION 3.7.2)

project(CompositeEngine)

file(GLOB_RECURSE CPP_FILES "src/*.cpp")

include_directories(
	"src"
	"extern/FBX SDK/2018.1.1/include"
	"extern/freeglut-2.8.1/include"
	"extern/glew-1.10.0/include"
	"extern/glm-0.9.5.1"
	"extern/SDL2-2.0.5/include"
)

link_directories(
	"extern/freeglut-2.8.1/lib/x86/Debug"
	"extern/glew-1.10.0/lib/Debug/Win32"
	"extern/FBX SDK/2018.1.1/lib/vs2015/x86/debug"
	"extern/SDL2-2.0.5/VisualC/Win32/Debug"
)

set_source_files_properties(
	"src/shaders/SimplePixelShader.hlsl"
	PROPERTIES
		VS_SHADER_TYPE Pixel
		VS_SHADER_MODEL 4.0_level_9_3
		VS_SHADER_ENTRYPOINT SimplePixelShader
)

set_source_files_properties(
	"src/shaders/SimpleVertexShader.hlsl"
	PROPERTIES
		VS_SHADER_TYPE Vertex
		VS_SHADER_MODEL 4.0_level_9_3
		VS_SHADER_ENTRYPOINT SimpleVertexShader
)

execute_process(
	COMMAND
		MSBuild
		"../../extern/SDL2-2.0.5/VisualC/SDL.sln"
		/p:PlatformToolset=v141
		/p:Configuration=${CE_CONFIGURATION}
		/p:Platform=${CE_PLATFORM}
		/m
)

# TODO: Remove Win32
# TODO: Copy libfbxsdk.dll
# TODO: Remove freeglut/glew/glm
# TODO: SDL 2.0
# TODO: Figure out FBX SDK
add_executable(
	CompositeEngine
	#WIN32
		${CPP_FILES}
		"src/shaders/SimplePixelShader.hlsl"
		"src/shaders/SimpleVertexShader.hlsl")

target_link_libraries(
	CompositeEngine
		d3d11
		#dxgi
		d3dcompiler
		#winmm
		libfbxsdk
		SDL2
		SDL2main

		glew32sd
		freeglut_static
)

#install(TARGETS CompositeEngine DESTINATION bin)
