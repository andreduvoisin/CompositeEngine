include(ExternalProject)

# TODO: Change GSL_VERSION and GIT_REPOSITORY back to Microsoft once
# this issue is resolved: https://github.com/microsoft/GSL/issues/816 

set(GSL_VERSION "3d980415749a4cdd3c7f976e9128536416514571")
set(GSL_VERSION_STRING "gsl-${GSL_VERSION}")

set(GSL_ROOT_DIR "${EXTERN_DIR}/${GSL_VERSION_STRING}")

ExternalProject_Add(
	GSLExternal
	PREFIX ${GSL_VERSION_STRING}

	DOWNLOAD_DIR ${EXTERN_DIR}
	GIT_REPOSITORY "https://github.com/andreduvoisin/GSL.git"
	GIT_TAG ${GSL_VERSION}

	SOURCE_DIR ${GSL_ROOT_DIR}

	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

add_library(GSL INTERFACE)
add_dependencies(GSL GSLExternal)
target_include_directories(GSL INTERFACE "${GSL_ROOT_DIR}/include")
