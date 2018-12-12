set(RAPIDJSON_ROOT_DIR "${EXTERN_DIR}/rapidjson-67fac85")

function(BootstrapRapidJSON)
	IncludeRapidJSON()
endfunction(BootstrapRapidJSON)

function(IncludeRapidJSON)
	include_directories("${RAPIDJSON_ROOT_DIR}/include")
endfunction(IncludeRapidJSON)
