include(ExternalProject)

set(GLM_VERSION "0.9.8.5")
set(GLM_VERSION_STRING "glm-${GLM_VERSION}")

set(GLM_ROOT_DIR "${EXTERN_DIR}/${GLM_VERSION_STRING}")

ExternalProject_Add(
	GLMExternal
	PREFIX ${GLM_VERSION_STRING}

	DOWNLOAD_DIR ${EXTERN_DIR}
	URL "https://github.com/g-truc/glm/releases/download/${GLM_VERSION}/${GLM_VERSION_STRING}.zip"

	SOURCE_DIR ${GLM_ROOT_DIR}

	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

add_library(GLM INTERFACE)
add_dependencies(GLM GLMExternal)

target_include_directories(GLM INTERFACE "${GLM_ROOT_DIR}")
