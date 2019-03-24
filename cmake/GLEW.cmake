include(ExternalProject)

set(GLEW_VERSION "2.1.0")
set(GLEW_VERSION_STRING "glew-${GLEW_VERSION}")

# Swap this to change GLEW from a statically-linked library to a dynamically-linked library.
set(GLEW_STATIC 1)
#set(GLEW_DYNAMIC 1)

set(GLEW_ROOT_DIR "${EXTERN_DIR}/${GLEW_VERSION_STRING}")
set(GLEW_BUILD_DIR "${GLEW_ROOT_DIR}/build")
set(GLEW_MSVC_DIR "${GLEW_BUILD_DIR}/vc12")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(GLEW_CONFIGURATION "Debug")
	set(GLEW_LIB_CONFIGURATION "d")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	set(GLEW_CONFIGURATION "Release")
	set(GLEW_LIB_CONFIGURATION "")
endif()

if(OS_WINDOWS)
	if(CMAKE_SIZEOF_VOID_P MATCHES 8)
		set(GLEW_PLATFORM "x64")
	else()
		set(GLEW_PLATFORM "Win32")
	endif()
endif()

if(OS_WINDOWS)
	set(CONFIGURE_COMMAND "")
	set(BUILD_COMMAND
		MSBuild
			"${GLEW_MSVC_DIR}/glew.sln"
			/p:PlatformToolset=v142 # Default: v120
			/p:Configuration=${CMAKE_BUILD_TYPE}
			/p:Platform=${GLEW_PLATFORM}
			/m
	)
	if(GLEW_STATIC)
		set(BUILD_BYPRODUCTS "${GLEW_ROOT_DIR}/lib/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32s${GLEW_LIB_CONFIGURATION}.lib")
	elseif(GLEW_DYNAMIC)
		set(BUILD_BYPRODUCTS "${GLEW_ROOT_DIR}/lib/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32${GLEW_LIB_CONFIGURATION}.lib")
	endif()
elseif(OS_MACOSX)
	# TODO: Builds release always. Need to pass the CMAKE build var, which changes the lib output, so fix that too.
	set(CONFIGURE_COMMAND cmake ./cmake)
	set(BUILD_COMMAND make -j4)
	if(GLEW_STATIC)
		set(BUILD_BYPRODUCTS "${GLEW_BUILD_DIR}/lib/libGLEW.a")
	elseif(GLEW_DYNAMIC)
		set(BUILD_BYPRODUCTS "${GLEW_BUILD_DIR}/lib/libGLEW.dylib")
	endif()
endif()

ExternalProject_Add(
	GLEWExternal
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

add_library(GLEW INTERFACE)
add_dependencies(GLEW GLEWExternal)

target_include_directories(GLEW INTERFACE "${GLEW_ROOT_DIR}/include")

if(GLEW_STATIC)
	if(OS_WINDOWS)
		set(LIBRARY_STATIC "${GLEW_ROOT_DIR}/lib/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32s${GLEW_LIB_CONFIGURATION}.lib")
	elseif(OS_MACOSX)
		set(LIBRARY_STATIC "${GLEW_BUILD_DIR}/lib/libGLEW.a")
	endif()

	target_compile_definitions(GLEW INTERFACE GLEW_STATIC)
	target_link_libraries(GLEW INTERFACE ${LIBRARY_STATIC})
elseif(GLEW_DYNAMIC)
	if(OS_WINDOWS)
		set(LIBRARY_DYNAMIC "${GLEW_ROOT_DIR}/lib/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32${GLEW_LIB_CONFIGURATION}.lib")
	elseif(OS_MACOSX)
		set(LIBRARY_DYNAMIC "${GLEW_BUILD_DIR}/lib/libGLEW.dylib")
	endif()

	target_link_libraries(GLEW INTERFACE ${LIBRARY_DYNAMIC})
endif()

if(GLEW_DYNAMIC)
	if(OS_WINDOWS)
		install(
			FILES "${GLEW_ROOT_DIR}/bin/${GLEW_CONFIGURATION}/${GLEW_PLATFORM}/glew32${GLEW_LIB_CONFIGURATION}.dll"
			DESTINATION "${CMAKE_INSTALL_PREFIX}"
		)
	elseif(OS_MACOSX)
		install(
			FILES "${GLEW_BUILD_DIR}/lib/libGLEW.dylib"
			DESTINATION "${CMAKE_INSTALL_PREFIX}/CompositeEngine.app/Contents/Frameworks"
		)
	endif()
endif()
