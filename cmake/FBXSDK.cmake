include(ExternalProject)

set(FBXSDK_YEAR "2018")
set(FBXSDK_VERSION_NAME "2018.1.1")
set(FBXSDK_VERSION_FILE_PREFIX "fbx20181_1")

# Swap this to change FBX SDK from a statically-linked library to a dynamically-linked library.
set(FBXSDK_STATIC 1)
#set(FBXSDK_DYNAMIC 1)

if(OS_WINDOWS)
	set(FBXSDK_FILE_NAME "${FBXSDK_VERSION_FILE_PREFIX}_fbxsdk_vs2015_win")
	set(FBXSDK_FILE_EXTENSION ".exe")
elseif(OS_MACOSX)
	set(FBXSDK_FILE_NAME "${FBXSDK_VERSION_FILE_PREFIX}_fbxsdk_clang_mac")
	set(FBXSDK_FILE_EXTENSION ".pkg.tgz")
endif()

set(FBXSDK_ROOT_DIR "${EXTERN_DIR}/${FBXSDK_FILE_NAME}")
set(FBXSDK_CLANG_DIR "${FBXSDK_ROOT_DIR}/Contents/Applications/Autodesk/FBX SDK/${FBXSDK_VERSION_NAME}/lib/clang")
set(FBXSDK_MSVC_DIR "${FBXSDK_ROOT_DIR}/lib/vs2015")

if(${CE_CONFIGURATION} STREQUAL "Debug")
	set(FBXSDK_CONFIGURATION "debug")
elseif(${CE_CONFIGURATION} STREQUAL "Release")
	set(FBXSDK_CONFIGURATION "release")
endif()

if(OS_WINDOWS)
	if(${CE_PLATFORM} STREQUAL "Win32")
		set(FBXSDK_PLATFORM "x86")
	elseif(${CE_PLATFORM} STREQUAL "x64")
		set(FBXSDK_PLATFORM "x64")
	endif()
endif()

if(OS_WINDOWS)
	# References:
	# https://en.wikipedia.org/wiki/Nullsoft_Scriptable_Install_System
	# https://nsis.sourceforge.io/Docs/Chapter3.html
		
	string(REPLACE "/" "\\" FBXSDK_ROOT_DIR_WIN_BACKSLASH ${FBXSDK_ROOT_DIR})

	set(DOWNLOAD_NO_EXTRACT TRUE)
	set(BINARY_DIR ${FBXSDK_ROOT_DIR})
	set(CONFIGURE_COMMAND ${EXTERN_DIR}/${FBXSDK_FILE_NAME}${FBXSDK_FILE_EXTENSION} /S /D=${FBXSDK_ROOT_DIR_WIN_BACKSLASH})
	set(BUILD_BYPRODUCTS "${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk-md.lib")
elseif(OS_MACOSX)
	unset(DOWNLOAD_NO_EXTRACT)
	set(BINARY_DIR "${FBXSDK_ROOT_DIR}/Contents")
	set(CONFIGURE_COMMAND ${CMAKE_COMMAND} -E tar xzf "${FBXSDK_ROOT_DIR}/Contents/Archive.pax.gz")
	set(BUILD_BYPRODUCTS "${FBXSDK_CLANG_DIR}/${FBXSDK_CONFIGURATION}/libfbxsdk.a")
endif()

ExternalProject_Add(
	FBXSDKExternal
	PREFIX ${FBXSDK_FILE_NAME}

	DOWNLOAD_DIR ${EXTERN_DIR}
	URL "https://download.autodesk.com/us/fbx/${FBXSDK_YEAR}/${FBXSDK_VERSION_NAME}/${FBXSDK_FILE_NAME}${FBXSDK_FILE_EXTENSION}"
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

if(OS_WINDOWS)
	set(INCLUDE_DIR "${FBXSDK_ROOT_DIR}/include")
elseif(OS_MACOSX)
	set(INCLUDE_DIR "${FBXSDK_ROOT_DIR}/Contents/Applications/Autodesk/FBX SDK/${FBXSDK_VERSION_NAME}/include")
endif()

target_include_directories(FBXSDK INTERFACE ${INCLUDE_DIR})

if(FBXSDK_STATIC)
	if(OS_WINDOWS)
		# FBX SDK can be linked statically with either /MD or /MT.
		# opengl32.lib (and glu32.lib) link with /MD, so we can't link with /MT.
		# /MD[d] is included in CMAKE_CXX_FLAGS[_*] by default.
		set(LIBRARIES_STATIC "${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk-md.lib")
	elseif(OS_MACOSX)
		find_library(CORE_FOUNDATION_LIBRARY CoreFoundation)
		set(LIBRARIES_STATIC ${CORE_FOUNDATION_LIBRARY} "${FBXSDK_CLANG_DIR}/${FBXSDK_CONFIGURATION}/libfbxsdk.a")
	endif()
		
	target_link_libraries(FBXSDK INTERFACE ${LIBRARIES_STATIC})
elseif(FBXSDK_DYNAMIC)
	if(OS_WINDOWS)
		set(LIBRARY_DYNAMIC "${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk.lib")
		target_compile_definitions(FBXSDK INTERFACE FBXSDK_SHARED)
	elseif(OS_MACOSX)
		set(LIBRARY_DYNAMIC "${FBXSDK_CLANG_DIR}/${FBXSDK_CONFIGURATION}/libfbxsdk.dylib")
	endif()
		
	target_link_libraries(FBXSDK INTERFACE ${LIBRARY_DYNAMIC})
endif()

if(FBXSDK_DYNAMIC)
	if(OS_WINDOWS)
		set(LIBRARY_DYNAMIC_SRC "${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk.dll")
		set(LIBRARY_DYNAMIC_DST "${PROJECT_BINARY_DIR}/${EXECUTABLE_SUBDIR}/${CE_CONFIGURATION}/libfbxsdk.dll")
	elseif(OS_MACOSX)
		set(LIBRARY_DYNAMIC_SRC "${FBXSDK_CLANG_DIR}/${FBXSDK_CONFIGURATION}/libfbxsdk.dylib")
		set(LIBRARY_DYNAMIC_DST "${PROJECT_BINARY_DIR}/engine/${CE_CONFIGURATION}/CompositeEngine.app/Contents/Frameworks/libfbxsdk.dylib")
	endif()
	
	install(
		FILES "${LIBRARY_DYNAMIC_SRC}"
		DESTINATION "${CMAKE_INSTALL_PREFIX}"
	)
endif()
