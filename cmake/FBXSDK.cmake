include(ExternalProject)

set(FBXSDK_YEAR "2018")
set(FBXSDK_VERSION_NAME "2018.1.1")
set(FBXSDK_VERSION_FILE_PREFIX "fbx20181_1")

# Swap this to change FBX SDK from a statically-linked library to a dynamically-linked library.
set(FBXSDK_STATIC 1)
#set(FBXSDK_DYNAMIC 1)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(FBXSDK_CONFIGURATION "debug")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	set(FBXSDK_CONFIGURATION "release")
endif()

if(OS_WINDOWS)
	set(FBXSDK_FILE_NAME "${FBXSDK_VERSION_FILE_PREFIX}_fbxsdk_vs2015_win")
	set(FBXSDK_FILE_EXTENSION ".exe")
elseif(OS_MACOSX)
	set(FBXSDK_FILE_NAME "${FBXSDK_VERSION_FILE_PREFIX}_fbxsdk_clang_mac")
	set(FBXSDK_FILE_EXTENSION ".pkg.tgz")
endif()
set(FBXSDK_FILE "${FBXSDK_FILE_NAME}${FBXSDK_FILE_EXTENSION}")

set(FBXSDK_ROOT_DIR "${EXTERN_DIR}/${FBXSDK_FILE_NAME}")

# References:
# https://en.wikipedia.org/wiki/Nullsoft_Scriptable_Install_System
# https://nsis.sourceforge.io/Docs/Chapter3.html
if(OS_WINDOWS)
	set(FBXSDK_MSVC_DIR "${FBXSDK_ROOT_DIR}/lib/vs2015")
	string(REPLACE "/" "\\" FBXSDK_ROOT_DIR_WIN_BACKSLASH ${FBXSDK_ROOT_DIR})

	set(DOWNLOAD_NO_EXTRACT TRUE)
	set(BINARY_DIR ${FBXSDK_ROOT_DIR})
	set(CONFIGURE_COMMAND "${EXTERN_DIR}/${FBXSDK_FILE}" /S /D=${FBXSDK_ROOT_DIR_WIN_BACKSLASH})
	set(BUILD_BYPRODUCTS "${FBXSDK_MSVC_DIR}/x64/${FBXSDK_CONFIGURATION}/libfbxsdk-md.lib")
	
	set(INCLUDE_DIRECTORIES "${FBXSDK_ROOT_DIR}/include")
	
	if(FBXSDK_STATIC)
		# FBX SDK can be linked statically with either /MD or /MT.
		# opengl32.lib (and glu32.lib) link with /MD, so we can't link with /MT.
		# /MD[d] is included in CMAKE_CXX_FLAGS[_*] by default.
		set(LINK_LIBRARIES "${FBXSDK_MSVC_DIR}/x64/${FBXSDK_CONFIGURATION}/libfbxsdk-md.lib")
	elseif(FBXSDK_DYNAMIC)
		set(LINK_LIBRARIES "${FBXSDK_MSVC_DIR}/x64/${FBXSDK_CONFIGURATION}/libfbxsdk.lib")
		target_compile_definitions(FBXSDK INTERFACE FBXSDK_SHARED)
		install(
			FILES "${FBXSDK_MSVC_DIR}/x64/${FBXSDK_CONFIGURATION}/libfbxsdk.dll"
			DESTINATION "${CMAKE_INSTALL_PREFIX}")
	endif()
endif()

if(OS_MACOSX)
	set(FBXSDK_CLANG_DIR "${FBXSDK_ROOT_DIR}/Contents/Applications/Autodesk/FBX SDK/${FBXSDK_VERSION_NAME}/lib/clang")

	unset(DOWNLOAD_NO_EXTRACT)
	set(BINARY_DIR "${FBXSDK_ROOT_DIR}/Contents")
	set(CONFIGURE_COMMAND ${CMAKE_COMMAND} -E tar xzf "${FBXSDK_ROOT_DIR}/Contents/Archive.pax.gz")
	set(BUILD_BYPRODUCTS "${FBXSDK_CLANG_DIR}/${FBXSDK_CONFIGURATION}/libfbxsdk.a")
	
	set(INCLUDE_DIRECTORIES "${FBXSDK_ROOT_DIR}/Contents/Applications/Autodesk/FBX SDK/${FBXSDK_VERSION_NAME}/include")
	
	if(FBXSDK_STATIC)
		find_library(CORE_FOUNDATION_LIBRARY CoreFoundation)
		set(LINK_LIBRARIES ${CORE_FOUNDATION_LIBRARY} "${FBXSDK_CLANG_DIR}/${FBXSDK_CONFIGURATION}/libfbxsdk.a")
	elseif(FBXSDK_DYNAMIC)
		set(LINK_LIBRARIES "${FBXSDK_CLANG_DIR}/${FBXSDK_CONFIGURATION}/libfbxsdk.dylib")
		install(
			FILES "${FBXSDK_CLANG_DIR}/${FBXSDK_CONFIGURATION}/libfbxsdk.dylib"
			DESTINATION "${CMAKE_INSTALL_PREFIX}/CompositeEngine.app/Contents/Frameworks")
	endif()
endif()

ExternalProject_Add(
	FBXSDKExternal
	PREFIX ${FBXSDK_FILE_NAME}

	DOWNLOAD_DIR ${EXTERN_DIR}
	URL "https://download.autodesk.com/us/fbx/${FBXSDK_YEAR}/${FBXSDK_VERSION_NAME}/${FBXSDK_FILE}"
	DOWNLOAD_NO_EXTRACT ${DOWNLOAD_NO_EXTRACT}

	SOURCE_DIR ${FBXSDK_ROOT_DIR}
	BINARY_DIR ${BINARY_DIR}

	CONFIGURE_COMMAND "${CONFIGURE_COMMAND}"
	BUILD_COMMAND ""
	INSTALL_COMMAND ""

	BUILD_BYPRODUCTS ${BUILD_BYPRODUCTS}
)

add_library(FBXSDK INTERFACE)
add_dependencies(FBXSDK FBXSDKExternal)
target_include_directories(FBXSDK INTERFACE ${INCLUDE_DIRECTORIES})
target_link_libraries(FBXSDK INTERFACE ${LINK_LIBRARIES})
