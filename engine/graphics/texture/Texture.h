#ifndef _CE_TEXTURE_H_
#define _CE_TEXTURE_H_

#include <vector>

namespace CE
{
	struct Texture
	{
		int width;
		int height;
		int channels;
		std::vector<std::byte> data;
	};

	typedef std::vector<Texture> Textures;
}

#endif // _CE_TEXTURE_H_