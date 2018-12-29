set(GLM_ROOT_DIR "${EXTERN_DIR}/glm-0.9.8.5")

function(BootstrapGLM)
	IncludeGLM()
endfunction()

function(IncludeGLM)
	include_directories("${GLM_ROOT_DIR}")
endfunction()
