include(ExternalProject)

set(STB_VERSION "e6afb9cbae4064da8c3e69af3ff5c4629579c1d2")
set(STB_VERSION_STRING "stb-${STB_VERSION}")

set(STB_ROOT_DIR "${EXTERN_DIR}/${STB_VERSION_STRING}")

# Currently, only stb_image is used. However, it is
# easiest to simply clone the whole git repository.
ExternalProject_Add(
	STBExternal
	PREFIX ${STB_VERSION_STRING}

	DOWNLOAD_DIR ${EXTERN_DIR}
	GIT_REPOSITORY "https://github.com/nothings/stb.git"
	GIT_TAG ${STB_VERSION}

	SOURCE_DIR ${STB_ROOT_DIR}

	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)

add_library(STB INTERFACE)
add_dependencies(STB STBExternal)

target_include_directories(STB INTERFACE "${STB_ROOT_DIR}")
