#ifndef _CE_FILE_2D_IMPORTER_H_
#define _CE_FILE_2D_IMPORTER_H_

namespace CE
{
	struct Texture;

	class File2DImporter
	{
	public:
		virtual bool ExtractTexture(const char* fileName, Texture* outTexture) = 0;
	};
}

#endif // _CE_FILE_2D_IMPORTER_H_