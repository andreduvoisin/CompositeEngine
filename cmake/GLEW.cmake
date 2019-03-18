include(ExternalProject)

set(GLEW_VERSION "2.1.0")
set(GLEW_VERSION_STRING "glew-${GLEW_VERSION}")

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
		# execute_process(
		#	COMMAND
		#		MSBuild
		#		"${GLEW_MSVC_DIR}/glew.sln"
		#		/p:PlatformToolset=v141 # Default: v120
		#		/p:Configuration=${CE_CONFIGURATION}
		#		/p:Platform=${CE_PLATFORM}
		#		/m
		# )

		ExternalProject_Add(
			GLEW
			PREFIX ${GLEW_VERSION_STRING}

			DOWNLOAD_DIR ${EXTERN_DIR}
			URL "https://github.com/nigels-com/glew/releases/download/${GLEW_VERSION_STRING}/${GLEW_VERSION_STRING}.tgz"

			SOURCE_DIR ${GLEW_ROOT_DIR}
			BINARY_DIR ${GLEW_BUILD_DIR}

			CONFIGURE_COMMAND ""
			BUILD_COMMAND MSBuild
				"${GLEW_MSVC_DIR}/glew.sln"
				/p:PlatformToolset=v141 # Default: v120
				/p:Configuration=${CE_CONFIGURATION}
				/p:Platform=${CE_PLATFORM}
				/m
			INSTALL_COMMAND ""

			BUILD_BYPRODUCTS
				"${GLEW_ROOT_DIR}/lib/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32s${GLEW_LIB_CONFIGURATION}.lib"
		)
	elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
		#add_subdirectory("${GLEW_BUILD_DIR}/cmake")
		# TODO: Builds release always, need to pass the CMAKE build var, which changes the lib output so fix that too.
		ExternalProject_Add(
			GLEW
			PREFIX ${GLEW_VERSION_STRING}

			DOWNLOAD_DIR ${EXTERN_DIR}
			URL "https://github.com/nigels-com/glew/releases/download/${GLEW_VERSION_STRING}/${GLEW_VERSION_STRING}.tgz"

			SOURCE_DIR ${GLEW_ROOT_DIR}
			BINARY_DIR ${GLEW_BUILD_DIR}

			CONFIGURE_COMMAND cmake ./cmake
			BUILD_COMMAND make -j4
			INSTALL_COMMAND ""

			BUILD_BYPRODUCTS
				"${GLEW_BUILD_DIR}/lib/libGLEW.a"
		)
	endif()
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
endfunction()

function(CopyGLEWFiles EXECUTABLE_SUBDIR)
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		# Dynamically-Linked Library
		#configure_file(
		#	"${GLEW_ROOT_DIR}/bin/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32${GLEW_LIB_CONFIGURATION}.dll"
		#	"${PROJECT_BINARY_DIR}/${EXECUTABLE_SUBDIR}/${CE_CONFIGURATION}/glew32${GLEW_LIB_CONFIGURATION}.dll"
		#	COPYONLY
		#)
	endif()
endfunction()
