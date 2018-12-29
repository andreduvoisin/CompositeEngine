cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0077 NEW)

# Specify the CEF distribution version.
set(CEF_VERSION "3.3538.1852.gcb937fc")

# Determine the platform.
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
	set(CEF_PLATFORM "macosx64")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	if(CMAKE_SIZEOF_VOID_P MATCHES 8)
		set(CEF_PLATFORM "linux64")
	else()
		set(CEF_PLATFORM "linux32")
	endif()
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	if(CMAKE_SIZEOF_VOID_P MATCHES 8)
		set(CEF_PLATFORM "windows64")
	else()
		set(CEF_PLATFORM "windows32")
	endif()
endif()

set(CEF_DISTRIBUTION "cef_binary_${CEF_VERSION}_${CEF_PLATFORM}")
set(CEF_ROOT "${EXTERN_DIR}/${CEF_DISTRIBUTION}")

if(${CE_CONFIGURATION} STREQUAL "Debug")
	set(CEF_CONFIGURATION "Debug")
elseif(${CE_CONFIGURATION} STREQUAL "Release")
	set(CEF_CONFIGURATION "Release")
endif()

# Reference: https://bitbucket.org/chromiumembedded/cef-project/src/master/cmake/DownloadCEF.cmake
#
# Download the CEF binary distribution for |CEF_PLATFORM| and |CEF_VERSION| to
# |DOWNLOAD_DIR|. The |CEF_ROOT| variable will be set in global scope pointing
# to the extracted location.
# Visit http://opensource.spotify.com/cefbuilds/index.html for the list of
# supported platforms and versions.
function(DownloadCEF DOWNLOAD_DIR)
	# Specify the binary distribution type and download directory.
	set(CEF_DISTRIBUTION "cef_binary_${CEF_VERSION}_${CEF_PLATFORM}")
	set(CEF_DOWNLOAD_DIR "${DOWNLOAD_DIR}")

	# The location where we expect the extracted binary distribution.
	set(CEF_ROOT "${CEF_DOWNLOAD_DIR}/${CEF_DISTRIBUTION}" CACHE INTERNAL "CEF_ROOT")

	# Download and/or extract the binary distribution if necessary.
	if(NOT IS_DIRECTORY "${CEF_ROOT}")
		set(CEF_DOWNLOAD_FILENAME "${CEF_DISTRIBUTION}.tar.bz2")
		set(CEF_DOWNLOAD_PATH "${CEF_DOWNLOAD_DIR}/${CEF_DOWNLOAD_FILENAME}")
		if(NOT EXISTS "${CEF_DOWNLOAD_PATH}")
			set(CEF_DOWNLOAD_URL "http://opensource.spotify.com/cefbuilds/${CEF_DOWNLOAD_FILENAME}")

			# Download the SHA1 hash for the binary distribution.
			message(STATUS "Downloading ${CEF_DOWNLOAD_PATH}.sha1...")
			file(DOWNLOAD "${CEF_DOWNLOAD_URL}.sha1" "${CEF_DOWNLOAD_PATH}.sha1")
			file(READ "${CEF_DOWNLOAD_PATH}.sha1" CEF_SHA1)

			# Download the binary distribution and verify the hash.
			message(STATUS "Downloading ${CEF_DOWNLOAD_PATH}...")
			file(
				DOWNLOAD "${CEF_DOWNLOAD_URL}" "${CEF_DOWNLOAD_PATH}"
				EXPECTED_HASH SHA1=${CEF_SHA1}
				SHOW_PROGRESS
			)
		endif()

		# Extract the binary distribution.
		message(STATUS "Extracting ${CEF_DOWNLOAD_PATH}...")
		execute_process(
			COMMAND ${CMAKE_COMMAND} -E tar xzf "${CEF_DOWNLOAD_DIR}/${CEF_DOWNLOAD_FILENAME}"
			WORKING_DIRECTORY ${CEF_DOWNLOAD_DIR}
		)
	endif()
endfunction()

# CMake Reference:
# https://bitbucket.org/chromiumembedded/cef-project/src/master/CMakeLists.txt
# https://bitbucket.org/chromiumembedded/cef/src/master/CMakeLists.txt.in
function(BuildCEF)
	ExternalProject_Add(
		CEF
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
			-DCMAKE_MAKE_PROGRAM=/Users/andreduvoisin/Downloads/ninja
			-DCEF_RUNTIME_LIBRARY_FLAG=/MD
			-DUSE_SANDBOX=OFF

		BUILD_COMMAND /Users/andreduvoisin/Downloads/ninja
		INSTALL_COMMAND ""

		BUILD_BYPRODUCTS
			"${CEF_ROOT}/libcef_dll_wrapper/libcef_dll_wrapper.a"
			"${CEF_ROOT}/${CEF_CONFIGURATION}/Chromium Embedded Framework.framework"
	)
endfunction()

function(BootstrapCEF TARGET_NAME EXECUTABLE_SUBDIR)
	IncludeCEF()
	LinkCEF(${TARGET_NAME})
	CopyCEFFiles(${TARGET_NAME} ${EXECUTABLE_SUBDIR})
endfunction()

function(IncludeCEF)
	include_directories(${CEF_ROOT})
endfunction()

# Reference: https://bitbucket.org/chromiumembedded/cef/wiki/LinkingDifferentRunTimeLibraries.md
# Sandbox support (linking cef_sandbox.lib) is only possible when your application is built with the /MT flag.
function(LinkCEF TARGET_NAME)
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		target_link_libraries(${TARGET_NAME} "${CEF_ROOT}/libcef_dll_wrapper/libcef_dll_wrapper.lib")
		target_link_libraries(${TARGET_NAME} "${CEF_ROOT}/${CEF_CONFIGURATION}/libcef.lib")

		#target_link_libraries(${TARGET_NAME} "${CEF_ROOT}/${CEF_CONFIGURATION}/cef_sandbox.lib")
	elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
		target_link_libraries(${TARGET_NAME} "${CEF_ROOT}/libcef_dll_wrapper/libcef_dll_wrapper.a")
		target_link_libraries(${TARGET_NAME} "${CEF_ROOT}/${CEF_CONFIGURATION}/Chromium Embedded Framework.framework")
		
		#target_link_libraries(${TARGET_NAME} "${CEF_ROOT}/${CEF_CONFIGURATION}/cef_sandbox.a")
	endif()
endfunction()

function(CopyCEFFiles TARGET_NAME EXECUTABLE_SUBDIR)
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		file(GLOB CEF_BINARY_FILES "${CEF_ROOT}/${CEF_CONFIGURATION}/*")
		file(
			COPY ${CEF_BINARY_FILES}
			DESTINATION "${PROJECT_BINARY_DIR}/${EXECUTABLE_SUBDIR}/${CE_CONFIGURATION}"
			FILES_MATCHING
				PATTERN "*.dll"
				PATTERN "*.bin"
		)

		file(GLOB CEF_RESOURCE_FILES "${CEF_ROOT}/Resources/*")
		file(
			COPY ${CEF_RESOURCE_FILES}
			DESTINATION "${PROJECT_BINARY_DIR}/${EXECUTABLE_SUBDIR}/${CE_CONFIGURATION}"
		)
	elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
		add_custom_command(
			TARGET ${TARGET_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
				"${CEF_ROOT}/${CEF_CONFIGURATION}/Chromium Embedded Framework.framework"
				"${PROJECT_BINARY_DIR}/engine/${CE_CONFIGURATION}/CompositeEngine.app/Contents/Frameworks/Chromium Embedded Framework.framework"
			VERBATIM
		)
	endif()
endfunction()
