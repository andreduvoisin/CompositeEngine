#ifndef _CE_ASSET_TRAITS_H_
#define _CE_ASSET_TRAITS_H_

namespace CE
{
	extern const char* const ASSET_FILE_HEADER;
	extern const size_t ASSET_FILE_HEADER_LENGTH;

	enum AssetType
	{
		SKELETON = 0,
		MESH,
		ANIMATION,
		TEXTURE
	};
}

#endif // _CE_ASSET_TRAITS_H_