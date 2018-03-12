#include "STBImageImporter.h"

#include "graphics\texture\Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace CE
{
	STBImageImporter::STBImageImporter()
	{
		stbi_set_flip_vertically_on_load(true);
	}

	bool STBImageImporter::ExtractTexture(const char* fileName, Texture* outTexture)
	{
		int width, height, channels;
		unsigned char* data = stbi_load(fileName, &width, &height, &channels, 0);

		if (!data)
		{
			return false;
		}

		outTexture->width = width;
		outTexture->height = height;
		outTexture->channels = channels;
		// todo: delete. also probs shouldn't be here anyways
		outTexture->data = new unsigned char[width * height * channels];
		memcpy(outTexture->data, data, width * height * channels);

		stbi_image_free(data);

		return true;
	}
}
