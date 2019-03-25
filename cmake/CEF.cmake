include(ExternalProject)

cmake_policy(SET CMP0074 NEW)
if(POLICY CMP0077)
	cmake_policy(SET CMP0077 NEW)
endif()

# Specify the CEF distribution version.
set(CEF_VERSION "3.3538.1852.gcb937fc")

# Determine the platform.
if(OS_MACOSX)
	set(CEF_PLATFORM "macosx64")
elseif(OS_LINUX)
	if(CMAKE_SIZEOF_VOID_P MATCHES 8)
		set(CEF_PLATFORM "linux64")
	else()
		set(CEF_PLATFORM "linux32")
	endif()
elseif(OS_WINDOWS)
	if(CMAKE_SIZEOF_VOID_P MATCHES 8)
		set(CEF_PLATFORM "windows64")
	else()
		set(CEF_PLATFORM "windows32")
	endif()
endif()

set(CEF_DISTRIBUTION "cef_binary_${CEF_VERSION}_${CEF_PLATFORM}")
set(CEF_ROOT "${EXTERN_DIR}/${CEF_DISTRIBUTION}")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(CEF_CONFIGURATION "Debug")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	set(CEF_CONFIGURATION "Release")
endif()

# CMake Reference:
# https://bitbucket.org/chromiumembedded/cef-project/src/master/CMakeLists.txt
# https://bitbucket.org/chromiumembedded/cef/src/master/CMakeLists.txt.in
if(OS_WINDOWS)
	set(BUILD_BYPRODUCTS
		"${CEF_ROOT}/libcef_dll_wrapper/libcef_dll_wrapper.dll"
		"${CEF_ROOT}/libcef_dll_wrapper/libcef_dll_wrapper.lib"
		"${CEF_ROOT}/${CEF_CONFIGURATION}/libcef.lib"
	)
elseif(OS_MACOSX)
	set(BUILD_BYPRODUCTS
		"${CEF_ROOT}/libcef_dll_wrapper/libcef_dll_wrapper.a"
		"${CEF_ROOT}/${CEF_CONFIGURATION}/Chromium Embedded Framework.framework"
	)
endif()
	
ExternalProject_Add(
	CEFExternal
	PREFIX ${CEF_DISTRIBUTION}

	DOWNLOAD_DIR ${EXTERN_DIR}
	URL "http://opensource.spotify.com/cefbuilds/${CEF_DISTRIBUTION}.tar.bz2"

	SOURCE_DIR ${CEF_ROOT}
	BINARY_DIR ${CEF_ROOT}

	# Custom configuration for Composite Engine.
	#
	# Linking Reference: https://bitbucket.org/chromiumembedded/cef/wiki/LinkingDifferentRunTimeLibraries.md
	# opengl32.lib (and glu32.lib) link with /MD, so we can't link with /MT.
	# /MD[d] is included in CMAKE_CXX_FLAGS[_*] by default.
	# Also, to maintain our sanity, we would like to avoid having to build Chromium and CEF.
	#
	# Sandbox Reference: https://magpcss.org/ceforum/viewtopic.php?f=6&t=15482
	CMAKE_ARGS
		-DCMAKE_MAKE_PROGRAM=ninja
		-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		-DCEF_RUNTIME_LIBRARY_FLAG=/MD
		-DUSE_SANDBOX=OFF

	BUILD_COMMAND ninja
	INSTALL_COMMAND ""

	BUILD_BYPRODUCTS ${BUILD_BYPRODUCTS}
)

add_library(CEF INTERFACE)
add_dependencies(CEF CEFExternal)

target_include_directories(CEF INTERFACE ${CEF_ROOT})

# Reference: https://bitbucket.org/chromiumembedded/cef/wiki/LinkingDifferentRunTimeLibraries.md
# Sandbox support (linking cef_sandbox.lib) is only possible when your application is built with the /MT flag.
if(OS_WINDOWS)
	set(LIBRARIES
		"${CEF_ROOT}/libcef_dll_wrapper/libcef_dll_wrapper.lib"
		"${CEF_ROOT}/${CEF_CONFIGURATION}/libcef.lib"
	)
	#set(LIBRARIES ${LIBRARIES} "${CEF_ROOT}/${CEF_CONFIGURATION}/cef_sandbox.lib")
elseif(OS_MACOSX)
	set(LIBRARIES
		"${CEF_ROOT}/libcef_dll_wrapper/libcef_dll_wrapper.a"
		"${CEF_ROOT}/${CEF_CONFIGURATION}/Chromium Embedded Framework.framework"
	)
	#set(LIBRARIES ${LIBRARIES} "${CEF_ROOT}/${CEF_CONFIGURATION}/cef_sandbox.a")
endif()

target_link_libraries(CEF INTERFACE ${LIBRARIES})

if(OS_WINDOWS)
	# The trailing slashes are important.
	# Reference: https://cmake.org/cmake/help/v3.12/command/install.html#installing-directories
	# "The last component of each directory name is appended to the destination directory but
	# a trailing slash may be used to avoid this because it leaves the last component empty."
	install(
		DIRECTORY "${CEF_ROOT}/${CEF_CONFIGURATION}/"
		DESTINATION "${CMAKE_INSTALL_PREFIX}"
		FILES_MATCHING
			PATTERN "*.dll"
			PATTERN "*.bin"
	)
	install(
		DIRECTORY "${CEF_ROOT}/Resources/"
		DESTINATION "${CMAKE_INSTALL_PREFIX}"
	)
elseif(OS_MACOSX)
	# TODO: This should use EXECUTABLE_SUBDIR?
	install(
		DIRECTORY "${CEF_ROOT}/${CEF_CONFIGURATION}/Chromium Embedded Framework.framework"
		DESTINATION "${CMAKE_INSTALL_PREFIX}/CompositeEngine.app/Contents/Frameworks"
	)
endif()
