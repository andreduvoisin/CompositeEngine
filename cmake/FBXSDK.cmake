include(ExternalProject)

set(FBXSDK_YEAR "2018")
set(FBXSDK_VERSION_NAME "2018.1.1")
set(FBXSDK_VERSION_FILE_PREFIX "fbx20181_1")

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
	set(FBXSDK_FILE_NAME "${FBXSDK_VERSION_FILE_PREFIX}_fbxsdk_clang_mac")
	set(FBXSDK_FILE_EXTENSION ".pkg.tgz")
endif()

set(FBXSDK_ROOT_DIR "${EXTERN_DIR}/${FBXSDK_FILE_NAME}")
set(FBXSDK_CLANG_DIR "${FBXSDK_ROOT_DIR}/Contents/Applications/Autodesk/FBX SDK/${FBXSDK_VERSION_NAME}/lib/clang")

if(${CE_CONFIGURATION} STREQUAL "Debug")
	set(FBXSDK_CONFIGURATION "debug")
elseif(${CE_CONFIGURATION} STREQUAL "Release")
	set(FBXSDK_CONFIGURATION "release")
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	if(${CE_PLATFORM} STREQUAL "Win32")
		set(FBXSDK_PLATFORM "x86")
	elseif(${CE_PLATFORM} STREQUAL "x64")
		set(FBXSDK_PLATFORM "x64")
	endif()
endif()

# Windows: https://en.wikipedia.org/wiki/Nullsoft_Scriptable_Install_System
function(BuildFBXSDK)
	ExternalProject_Add(
		FBXSDK
		PREFIX ${FBXSDK_FILE_NAME}

		DOWNLOAD_DIR ${EXTERN_DIR}
		URL "http://download.autodesk.com/us/fbx/${FBXSDK_YEAR}/${FBXSDK_VERSION_NAME}/${FBXSDK_FILE_NAME}${FBXSDK_FILE_EXTENSION}"

		SOURCE_DIR ${FBXSDK_ROOT_DIR}
		BINARY_DIR "${FBXSDK_ROOT_DIR}/Contents"

		CONFIGURE_COMMAND ${CMAKE_COMMAND} -E tar xzf "${FBXSDK_ROOT_DIR}/Contents/Archive.pax.gz"
		BUILD_COMMAND ""
		INSTALL_COMMAND ""

		BUILD_BYPRODUCTS
			"${FBXSDK_CLANG_DIR}/${FBXSDK_CONFIGURATION}/libfbxsdk.a"
	)
endfunction()

function(BootstrapFBXSDK TARGET_NAME EXECUTABLE_SUBDIR)
	IncludeFBXSDK()
	LinkFBXSDK(${TARGET_NAME})
	CopyFBXSDKFiles(${EXECUTABLE_SUBDIR})
endfunction()

function(IncludeFBXSDK)
	include_directories("${FBXSDK_ROOT_DIR}/Contents/Applications/Autodesk/FBX SDK/${FBXSDK_VERSION_NAME}/include")
endfunction()

# FBX SDK can be linked statically (with either /MD or /MT) or dynamically.
# opengl32.lib (and glu32.lib) link with /MD, so we can't link with /MT.
# /MD[d] is included in CMAKE_CXX_FLAGS[_*] by default.
function(LinkFBXSDK TARGET_NAME)
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		# Statically-Linked Library (/MD)
		target_link_libraries(${TARGET_NAME} "${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk-md.lib")

		# Dynamically-Linked Library
		#target_compile_definitions(${TARGET_NAME} PRIVATE FBXSDK_SHARED)
		#target_link_libraries(${TARGET_NAME} "${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk.lib")
	elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
		# Statically-Linked Library
		find_library(CORE_FOUNDATION_LIBRARY CoreFoundation)
		target_link_libraries(${TARGET_NAME} ${CORE_FOUNDATION_LIBRARY})
		target_link_libraries(${TARGET_NAME} "${FBXSDK_CLANG_DIR}/${FBXSDK_CONFIGURATION}/libfbxsdk.a")

		# Dynamically-Linked Library
		#target_link_libraries(${TARGET_NAME} "${FBXSDK_CLANG_DIR}/${FBXSDK_CONFIGURATION}/libfbxsdk.dylib")
	endif()
endfunction()

function(CopyFBXSDKFiles EXECUTABLE_SUBDIR)
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		# Dynamically-Linked Library
		#configure_file(
		#	"${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk.dll"
		#	"${PROJECT_BINARY_DIR}/${EXECUTABLE_SUBDIR}/${CE_CONFIGURATION}/libfbxsdk.dll"
		#	COPYONLY
		#)
	endif()
endfunction()
