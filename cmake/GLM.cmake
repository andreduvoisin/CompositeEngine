include(ExternalProject)

set(GLM_VERSION "0.9.8.5")
set(GLM_VERSION_STRING "glm-${GLM_VERSION}")

set(GLM_ROOT_DIR "${EXTERN_DIR}/${GLM_VERSION_STRING}")

function(BuildGLM)
	ExternalProject_Add(
		GLM
		PREFIX ${GLM_VERSION_STRING}

		DOWNLOAD_DIR ${EXTERN_DIR}
		URL "https://github.com/g-truc/glm/releases/download/${GLM_VERSION}/${GLM_VERSION_STRING}.zip"

		SOURCE_DIR ${GLM_ROOT_DIR}

		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
	)
endfunction()

function(BootstrapGLM)
	IncludeGLM()
endfunction()

function(IncludeGLM)
	include_directories("${GLM_ROOT_DIR}")
endfunction()
