#ifndef _CE_TEXTURE_H_
#define _CE_TEXTURE_H_

namespace CE
{
	struct Texture
	{
		int width;
		int height;
		int channels;
		unsigned char* data;
	};
}

#endif // _CE_TEXTURE_H_