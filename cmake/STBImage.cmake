set(STBIMAGE_ROOT_DIR "${EXTERN_DIR}/stb_image-2.16")

function(BootstrapSTBImage)
	IncludeSTBImage()
endfunction(BootstrapSTBImage)

function(IncludeSTBImage)
	include_directories("${STBIMAGE_ROOT_DIR}")
endfunction(IncludeSTBImage)
