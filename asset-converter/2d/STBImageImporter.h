#ifndef _CE_STB_IMAGE_IMPORTER_H_
#define _CE_STB_IMAGE_IMPORTER_H_

#include "graphics/File2DImporter.h"

namespace CE
{
    class STBImageImporter : public File2DImporter
    {
    public:
        STBImageImporter();

        bool ExtractTexture(const char* fileName, Texture* outTexture) override;
    };
}

#endif // _CE_STB_IMAGE_IMPORTER_H_
