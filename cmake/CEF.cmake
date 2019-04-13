include(ExternalProject)

cmake_policy(SET CMP0074 NEW)
if(POLICY CMP0077)
	cmake_policy(SET CMP0077 NEW)
endif()

set(CEF_VERSION "73.1.12+gee4b49f+chromium-73.0.3683.75")

if(OS_WINDOWS)
	set(CEF_PLATFORM "windows64")
elseif(OS_MACOSX)
	set(CEF_PLATFORM "macosx64")
endif()
set(CEF_DISTRIBUTION "cef_binary_${CEF_VERSION}_${CEF_PLATFORM}")
set(CEF_ROOT "${EXTERN_DIR}/${CEF_DISTRIBUTION}")

# The CEF build server requires the URL to be encoded, returning a 403 otherwise.
set(CEF_URL "http://opensource.spotify.com/cefbuilds/${CEF_DISTRIBUTION}.tar.bz2")
string(REPLACE "+" "%2B" CEF_ENCODED_URL ${CEF_URL})

# CMake Reference:
# https://bitbucket.org/chromiumembedded/cef-project/src/master/CMakeLists.txt
# https://bitbucket.org/chromiumembedded/cef/src/master/CMakeLists.txt.in
if(OS_WINDOWS)
	set(BUILD_BYPRODUCTS
		"${CEF_ROOT}/libcef_dll_wrapper/libcef_dll_wrapper.dll"
		"${CEF_ROOT}/libcef_dll_wrapper/libcef_dll_wrapper.lib"
		"${CEF_ROOT}/${CMAKE_BUILD_TYPE}/libcef.lib")

	set(LIBRARIES
		"${CEF_ROOT}/libcef_dll_wrapper/libcef_dll_wrapper.lib"
		"${CEF_ROOT}/${CMAKE_BUILD_TYPE}/libcef.lib")
	#set(LIBRARIES ${LIBRARIES} "${CEF_ROOT}/${CMAKE_BUILD_TYPE}/cef_sandbox.lib")
	
	# The trailing slashes are important.
	# Reference: https://cmake.org/cmake/help/v3.12/command/install.html#installing-directories
	# "The last component of each directory name is appended to the destination directory but
	# a trailing slash may be used to avoid this because it leaves the last component empty."
	install(
		DIRECTORY "${CEF_ROOT}/${CMAKE_BUILD_TYPE}/"
		DESTINATION "${CMAKE_INSTALL_PREFIX}"
		FILES_MATCHING
			PATTERN "*.dll"
			PATTERN "*.bin")
	install(
		DIRECTORY "${CEF_ROOT}/Resources/"
		DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()

if(OS_MACOSX)
	set(BUILD_BYPRODUCTS
		"${CEF_ROOT}/libcef_dll_wrapper/libcef_dll_wrapper.a"
		"${CEF_ROOT}/${CMAKE_BUILD_TYPE}/Chromium Embedded Framework.framework")
	
	set(LIBRARIES "${CEF_ROOT}/libcef_dll_wrapper/libcef_dll_wrapper.a")
	#set(LIBRARIES ${LIBRARIES} "${CEF_ROOT}/${CMAKE_BUILD_TYPE}/cef_sandbox.a")
	
	# TODO: This should use EXECUTABLE_SUBDIR?
	install(
		DIRECTORY "${CEF_ROOT}/${CMAKE_BUILD_TYPE}/Chromium Embedded Framework.framework"
		DESTINATION "${CMAKE_INSTALL_PREFIX}/CompositeEngine.app/Contents/Frameworks")
	# TODO: Shouldn't be including this framework twice...
	install(
		DIRECTORY "${CEF_ROOT}/${CMAKE_BUILD_TYPE}/Chromium Embedded Framework.framework"
		DESTINATION "${CMAKE_INSTALL_PREFIX}/CompositeEngine.app/Contents/Frameworks/CompositeCefSubprocess.app/Contents/Frameworks")
endif()
	
ExternalProject_Add(
	CEFExternal
	PREFIX ${CEF_DISTRIBUTION}

	DOWNLOAD_DIR ${EXTERN_DIR}
	URL ${CEF_ENCODED_URL}

	SOURCE_DIR ${CEF_ROOT}
	BINARY_DIR ${CEF_ROOT}

	# Custom configuration for Composite Engine.
	#
	# Linking Reference: https://bitbucket.org/chromiumembedded/cef/wiki/LinkingDifferentRunTimeLibraries.md
	# opengl32.lib (and glu32.lib) link with /MD, so we can't link with /MT.
	# /MD[d] is included in CMAKE_CXX_FLAGS[_*] by default.
	# Sandbox support (linking cef_sandbox.lib) is only possible when your application is built with the /MT flag.
	# Also, to maintain our sanity, we would like to avoid having to build Chromium and CEF.
	#
	# Sandbox Reference: https://magpcss.org/ceforum/viewtopic.php?f=6&t=15482
	CMAKE_ARGS
		-DCMAKE_MAKE_PROGRAM=ninja
		-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		-DCEF_RUNTIME_LIBRARY_FLAG=/MD
		-DPROJECT_ARCH=x86_64
		-DUSE_SANDBOX=OFF

	BUILD_COMMAND ninja
	INSTALL_COMMAND ""

	BUILD_BYPRODUCTS ${BUILD_BYPRODUCTS}
)

add_library(CEF INTERFACE)
add_dependencies(CEF CEFExternal)
target_include_directories(CEF INTERFACE ${CEF_ROOT})
target_link_libraries(CEF INTERFACE ${LIBRARIES})
