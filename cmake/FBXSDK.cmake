set(FBXSDK_ROOT_DIR "${EXTERN_DIR}/FBX SDK/2018.1.1")
set(FBXSDK_MSVC_DIR "${FBXSDK_ROOT_DIR}/lib/vs2015")
	
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

function(BootstrapFBXSDK TARGET_NAME EXECUTABLE_SUBDIR)
	IncludeFBXSDK()
	LinkFBXSDK(${TARGET_NAME})
	CopyFBXSDKFiles(${EXECUTABLE_SUBDIR})
endfunction(BootstrapFBXSDK)

function(IncludeFBXSDK)
	include_directories("${FBXSDK_ROOT_DIR}/include")
endfunction(IncludeFBXSDK)

# FBX SDK can be linked statically (with either /MD or /MT) or dynamically.
# opengl32.lib (and glu32.lib) link with /MD, so we can't link with /MT.
# /MD[d] is included in CMAKE_CXX_FLAGS[_*] by default.
function(LinkFBXSDK TARGET_NAME)
	# Statically-Linked Library (/MD)
	target_link_libraries(${TARGET_NAME} "${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk-md.lib")

	# Dynamically-Linked Library
	#target_compile_definitions(${TARGET_NAME} PRIVATE FBXSDK_SHARED)
	#target_link_libraries(${TARGET_NAME} "${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk.lib")
endfunction(LinkFBXSDK)

function(CopyFBXSDKFiles EXECUTABLE_SUBDIR)
	# Dynamically-Linked Library
	#configure_file(
	#	"${FBXSDK_MSVC_DIR}/${FBXSDK_PLATFORM}/${FBXSDK_CONFIGURATION}/libfbxsdk.dll"
	#	"${PROJECT_BINARY_DIR}/${EXECUTABLE_SUBDIR}/${CE_CONFIGURATION}/libfbxsdk.dll"
	#	COPYONLY
	#)
endfunction(CopyFBXSDKFiles)
