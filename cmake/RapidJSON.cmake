include(ExternalProject)

set(RAPIDJSON_VERSION "67fac85")
set(RAPIDJSON_VERSION_STRING "rapidjson-${RAPIDJSON_VERSION}")

set(RAPIDJSON_ROOT_DIR "${EXTERN_DIR}/${RAPIDJSON_VERSION_STRING}")

function(BuildRapidJSON)
	ExternalProject_Add(
		RapidJSON
		PREFIX ${RAPIDJSON_VERSION_STRING}

		DOWNLOAD_DIR ${EXTERN_DIR}
		GIT_REPOSITORY "https://github.com/Tencent/rapidjson.git"
		GIT_TAG ${RAPIDJSON_VERSION}

		SOURCE_DIR ${RAPIDJSON_ROOT_DIR}

		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
	)
endfunction()

function(BootstrapRapidJSON)
	IncludeRapidJSON()
endfunction()

function(IncludeRapidJSON)
	include_directories("${RAPIDJSON_ROOT_DIR}/include")
endfunction()
