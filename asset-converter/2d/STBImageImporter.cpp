#include "STBImageImporter.h"

#include "graphics/texture/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace CE
{
    STBImageImporter::STBImageImporter()
    {
        stbi_set_flip_vertically_on_load(1);
    }

    bool STBImageImporter::ExtractTexture(const char* fileName, Texture* outTexture)
    {
        int width;
        int height;
        int channels;
        unsigned char* data = stbi_load(fileName, &width, &height, &channels, 0);

        if (data == nullptr)
        {
            return false;
        }

        outTexture->width = width;
        outTexture->height = height;
        outTexture->channels = channels;

        // TODO: Should this be allocating memory?
        int size = width * height * channels;
        outTexture->data = std::vector<std::byte>(size);
        memcpy(outTexture->data.data(), data, size);

        stbi_image_free(data);

        return true;
    }
}
