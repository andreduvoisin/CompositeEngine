set(GLEW_ROOT_DIR "${EXTERN_DIR}/glew-2.1.0")
set(GLEW_BUILD_DIR "${GLEW_ROOT_DIR}/build")
set(GLEW_MSVC_DIR "${GLEW_BUILD_DIR}/vc12")

if(${CE_CONFIGURATION} STREQUAL "Debug")
	set(GLEW_CONFIGURATION "Debug")
	set(GLEW_LIB_CONFIGURATION "d")
elseif(${CE_CONFIGURATION} STREQUAL "Release")
	set(GLEW_CONFIGURATION "Release")
	set(GLEW_LIB_CONFIGURATION "")
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	if(${CE_PLATFORM} STREQUAL "Win32")
		set(GLEW_PLATFORM "Win32")
	elseif(${CE_PLATFORM} STREQUAL "x64")
		set(GLEW_PLATFORM "x64")
	endif()
endif()

function(BuildGLEW)
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		# TODO: Does this need WORKING_DIRECTORY? Windows outputs to libs/, whereas Mac outputs to build/libs/
		execute_process(
			COMMAND
				MSBuild
				"${GLEW_MSVC_DIR}/glew.sln"
				/p:PlatformToolset=v141 # Default: v120
				/p:Configuration=${CE_CONFIGURATION}
				/p:Platform=${CE_PLATFORM}
				/m
		)
	elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
		# TODO: How is Debug vs Release specified?
		execute_process(COMMAND cmake ./cmake WORKING_DIRECTORY "${GLEW_BUILD_DIR}")
		execute_process(COMMAND make -j4 WORKING_DIRECTORY "${GLEW_BUILD_DIR}")
	endif()
endfunction(BuildGLEW)

function(BootstrapGLEW TARGET_NAME EXECUTABLE_SUBDIR)
	IncludeGLEW()
	LinkGLEW(${TARGET_NAME})
	CopyGLEWFiles(${EXECUTABLE_SUBDIR})
endfunction(BootstrapGLEW)

function(IncludeGLEW)
	include_directories("${GLEW_ROOT_DIR}/include")
endfunction(IncludeGLEW)

function(LinkGLEW TARGET_NAME)
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		# Statically-Linked Library
		target_compile_definitions(CompositeEngine PRIVATE GLEW_STATIC)
		target_link_libraries(CompositeEngine "${GLEW_ROOT_DIR}/lib/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32s${GLEW_LIB_CONFIGURATION}.lib")

		# Dynamically-Linked Library
		#target_link_libraries(CompositeEngine "${GLEW_ROOT_DIR}/lib/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32${GLEW_LIB_CONFIGURATION}.lib")
	elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
		# Statically-Linked Library
		target_compile_definitions(CompositeEngine PRIVATE GLEW_STATIC)
		target_link_libraries(CompositeEngine "${GLEW_BUILD_DIR}/lib/libGLEW.a")

		# Dynamically-Linked Library
		#target_link_libraries(CompositeEngine "${GLEW_BUILD_DIR}/lib/libGLEW.dylib")
	endif()
endfunction(LinkGLEW)

function(CopyGLEWFiles EXECUTABLE_SUBDIR)
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		# Dynamically-Linked Library
		#configure_file(
		#	"${GLEW_ROOT_DIR}/bin/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32${GLEW_LIB_CONFIGURATION}.dll"
		#	"${PROJECT_BINARY_DIR}/${EXECUTABLE_SUBDIR}/${CE_CONFIGURATION}/glew32${GLEW_LIB_CONFIGURATION}.dll"
		#	COPYONLY
		#)
	endif()
endfunction(CopyGLEWFiles)
