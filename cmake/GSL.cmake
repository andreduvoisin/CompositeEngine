include(ExternalProject)

set(GSL_VERSION "1212beae777dba02c230ece8c0c0ec12790047ea")
set(GSL_VERSION_STRING "gsl-${GSL_VERSION}")

set(GSL_ROOT_DIR "${EXTERN_DIR}/${GSL_VERSION_STRING}")

ExternalProject_Add(
	GSLExternal
	PREFIX ${GSL_VERSION_STRING}

	DOWNLOAD_DIR ${EXTERN_DIR}
	GIT_REPOSITORY "https://github.com/microsoft/GSL.git"
	GIT_TAG ${GSL_VERSION}

	SOURCE_DIR ${GSL_ROOT_DIR}

	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

add_library(GSL INTERFACE)
add_dependencies(GSL GSLExternal)
target_include_directories(GSL INTERFACE "${GSL_ROOT_DIR}/include")
