set(CEF_VERSION_DIR_NAME "cef_binary_3.3538.1852.gcb937fc")
set(CEF_PLATFORM_DIR_NAME "windows64")

set(CEF_ROOT_DIR "${EXTERN_DIR}/${CEF_VERSION_DIR_NAME}")
set(CEF_PLATFORM_DIR "${CEF_ROOT_DIR}/${CEF_PLATFORM_DIR_NAME}")

if(${CE_CONFIGURATION} STREQUAL "Debug")
	set(CEF_CONFIGURATION "Debug")
elseif(${CE_CONFIGURATION} STREQUAL "Release")
	set(CEF_CONFIGURATION "Release")
endif()

if(${CE_PLATFORM} STREQUAL "Win32")
	set(CEF_PLATFORM "Win32")
elseif(${CE_PLATFORM} STREQUAL "x64")
	set(CEF_PLATFORM "x64")
endif()

# Reference: https://bitbucket.org/chromiumembedded/cef/wiki/LinkingDifferentRunTimeLibraries.md
#
# opengl32.lib (and glu32.lib) link with /MD, so we can't link with /MT.
# /MD[d] is included in CMAKE_CXX_FLAGS[_*] by default.
#
# We also would like to avoid having to build all of Chromium and CEF for our sanity's sake.
function(BuildCEF)
	set(CEF_RUNTIME_LIBRARY_FLAG "/MD")

	# Reference: https://magpcss.org/ceforum/viewtopic.php?f=6&t=15482
	set(USE_SANDBOX OFF)

	add_subdirectory(${CEF_PLATFORM_DIR})
endfunction(BuildCEF)

function(BootstrapCEF TARGET_NAME EXECUTABLE_SUBDIR)
	IncludeCEF()
	LinkCEF(${TARGET_NAME})
	CopyCEFFiles(${EXECUTABLE_SUBDIR})
endfunction(BootstrapCEF)

function(IncludeCEF)
	include_directories("${CEF_PLATFORM_DIR}")
endfunction(IncludeCEF)

function(LinkCEF TARGET_NAME)
	target_link_libraries(${TARGET_NAME} "${PROJECT_BINARY_DIR}/extern/${CEF_VERSION_DIR_NAME}/${CEF_PLATFORM_DIR_NAME}/libcef_dll_wrapper/${CEF_CONFIGURATION}/libcef_dll_wrapper.lib")
	target_link_libraries(${TARGET_NAME} "${CEF_PLATFORM_DIR}/${CEF_CONFIGURATION}/libcef.lib")

	# Sandbox support (linking cef_sandbox.lib) is only possible when your application is built with the /MT flag.
	#target_link_libraries(${TARGET_NAME} "${CEF_PLATFORM_DIR}/${CEF_CONFIGURATION}/cef_sandbox.lib")
endfunction(LinkCEF)

function(CopyCEFFiles EXECUTABLE_SUBDIR)
	file(GLOB CEF_BINARY_FILES "${CEF_PLATFORM_DIR}/${CEF_CONFIGURATION}/*")
	file(
		COPY ${CEF_BINARY_FILES}
		DESTINATION "${PROJECT_BINARY_DIR}/${EXECUTABLE_SUBDIR}/${CE_CONFIGURATION}"
		FILES_MATCHING
			PATTERN "*.dll"
			PATTERN "*.bin"
	)

	file(GLOB CEF_RESOURCE_FILES "${CEF_PLATFORM_DIR}/Resources/*")
	file(
		COPY ${CEF_RESOURCE_FILES}
		DESTINATION "${PROJECT_BINARY_DIR}/${EXECUTABLE_SUBDIR}/${CE_CONFIGURATION}"
	)
endfunction(CopyCEFFiles)
