include(ExternalProject)

set(GSL_VERSION "v2.0.0")
set(GSL_VERSION_STRING "gsl-${GSL_VERSION}")

set(GSL_ROOT_DIR "${EXTERN_DIR}/${GSL_VERSION_STRING}")

ExternalProject_Add(
	GSLExternal
	PREFIX ${GSL_VERSION_STRING}

	DOWNLOAD_DIR ${EXTERN_DIR}
	URL "https://github.com/microsoft/GSL/archive/${GSL_VERSION}.zip"

	SOURCE_DIR ${GSL_ROOT_DIR}

	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

add_library(GSL INTERFACE)
add_dependencies(GSL GSLExternal)
target_include_directories(GSL INTERFACE "${GSL_ROOT_DIR}/include")
