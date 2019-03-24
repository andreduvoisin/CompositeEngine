#include "AssetTraits.h"

#include <cstddef>
#include <cstring>

namespace CE
{
	const char* const ASSET_FILE_HEADER = "CEASSET"; // includes '\0'
	const size_t ASSET_FILE_HEADER_LENGTH = strlen(ASSET_FILE_HEADER) + 1; // '\0'
}
