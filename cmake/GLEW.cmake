include(ExternalProject)

set(GLEW_VERSION "2.1.0")
set(GLEW_VERSION_STRING "glew-${GLEW_VERSION}")

# Swap this to change GLEW from a statically-linked library to a dynamically-linked library.
set(GLEW_STATIC 1)
#set(GLEW_DYNAMIC 1)

set(GLEW_ROOT_DIR "${EXTERN_DIR}/${GLEW_VERSION_STRING}")
set(GLEW_BUILD_DIR "${GLEW_ROOT_DIR}/build")
set(GLEW_MSVC_DIR "${GLEW_BUILD_DIR}/vc12")

if(${CE_CONFIGURATION} STREQUAL "Debug")
	set(GLEW_CONFIGURATION "Debug")
	set(GLEW_LIB_CONFIGURATION "d")
elseif(${CE_CONFIGURATION} STREQUAL "Release")
	set(GLEW_CONFIGURATION "Release")
	set(GLEW_LIB_CONFIGURATION "")
endif()

if(OS_WINDOWS)
	if(${CE_PLATFORM} STREQUAL "Win32")
		set(GLEW_PLATFORM "Win32")
	elseif(${CE_PLATFORM} STREQUAL "x64")
		set(GLEW_PLATFORM "x64")
	endif()
endif()

function(BuildGLEW)
	if(OS_WINDOWS)
		set(CONFIGURE_COMMAND "")
		set(BUILD_COMMAND
			MSBuild
				"${GLEW_MSVC_DIR}/glew.sln"
				/p:PlatformToolset=v141 # Default: v120
				/p:Configuration=${CE_CONFIGURATION}
				/p:Platform=${CE_PLATFORM}
				/m
		)
		set(BUILD_BYPRODUCTS "${GLEW_ROOT_DIR}/lib/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32s${GLEW_LIB_CONFIGURATION}.lib")
	elseif(OS_MACOSX)
		# TODO: Builds release always. Need to pass the CMAKE build var, which changes the lib output, so fix that too.
		set(CONFIGURE_COMMAND cmake ./cmake)
		set(BUILD_COMMAND make -j4)
		set(BUILD_BYPRODUCTS "${GLEW_BUILD_DIR}/lib/libGLEW.a")
	endif()

	ExternalProject_Add(
		GLEW
		PREFIX ${GLEW_VERSION_STRING}

		DOWNLOAD_DIR ${EXTERN_DIR}
		URL "https://github.com/nigels-com/glew/releases/download/${GLEW_VERSION_STRING}/${GLEW_VERSION_STRING}.tgz"

		SOURCE_DIR ${GLEW_ROOT_DIR}
		BINARY_DIR ${GLEW_BUILD_DIR}

		CONFIGURE_COMMAND "${CONFIGURE_COMMAND}"
		BUILD_COMMAND "${BUILD_COMMAND}"
		INSTALL_COMMAND ""

		BUILD_BYPRODUCTS ${BUILD_BYPRODUCTS}
	)
endfunction()

function(BootstrapGLEW TARGET_NAME EXECUTABLE_SUBDIR)
	IncludeGLEW()
	LinkGLEW(${TARGET_NAME})
	CopyGLEWFiles(${EXECUTABLE_SUBDIR})
endfunction()

function(IncludeGLEW)
	include_directories("${GLEW_ROOT_DIR}/include")
endfunction()

function(LinkGLEW TARGET_NAME)
	if(GLEW_STATIC)
		if(OS_WINDOWS)
			set(LIBRARY_STATIC "${GLEW_ROOT_DIR}/lib/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32s${GLEW_LIB_CONFIGURATION}.lib")
		elseif(OS_MACOSX)
			set(LIBRARY_STATIC "${GLEW_BUILD_DIR}/lib/libGLEW.a")
		endif()

		target_compile_definitions(CompositeEngine PRIVATE GLEW_STATIC)
		target_link_libraries(CompositeEngine PRIVATE ${LIBRARY_STATIC})
	elseif(GLEW_DYNAMIC)
		if(OS_WINDOWS)
			set(LIBRARY_DYNAMIC "${GLEW_ROOT_DIR}/lib/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32${GLEW_LIB_CONFIGURATION}.lib")
		elseif(OS_MACOSX)
			set(LIBRARY_DYNAMIC "${GLEW_BUILD_DIR}/lib/libGLEW.dylib")
		endif()

		target_link_libraries(CompositeEngine PRIVATE ${LIBRARY_DYNAMIC})
	endif()
endfunction()

function(CopyGLEWFiles EXECUTABLE_SUBDIR)
	if(GLEW_DYNAMIC)
		if(OS_WINDOWS)
			set(LIBRARY_DYNAMIC_SRC "${GLEW_ROOT_DIR}/bin/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32${GLEW_LIB_CONFIGURATION}.dll")
			set(LIBRARY_DYNAMIC_DST "${PROJECT_BINARY_DIR}/${EXECUTABLE_SUBDIR}/${CE_CONFIGURATION}/glew32${GLEW_LIB_CONFIGURATION}.dll")
		elseif(OS_MACOSX)
			set(LIBRARY_DYNAMIC_SRC "${GLEW_BUILD_DIR}/lib/libGLEW.dylib")
			set(LIBRARY_DYNAMIC_DST "${PROJECT_BINARY_DIR}/engine/${CE_CONFIGURATION}/CompositeEngine.app/Contents/Frameworks/libGLEW.dylib")
		endif()

		add_custom_command(
			TARGET ${TARGET_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
				${LIBRARY_DYNAMIC_SRC}
				${LIBRARY_DYNAMIC_DST}
			VERBATIM
		)
	endif()
endfunction()
