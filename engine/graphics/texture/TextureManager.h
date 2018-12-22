#ifndef _CE_TEXTURE_MANAGER_H_
#define _CE_TEXTURE_MANAGER_H_

#include "common/Singleton.h"

#include <string>
#include <unordered_map>

namespace CE
{
	struct Texture;
	class File2DImporter;

	class TextureManager : public Singleton<TextureManager>
	{
	public:
		void Initialize(File2DImporter* importer);
		void Destroy();

		Texture* GetTexture(const char* szTextureFile);

	private:
		File2DImporter* m_importer;
		std::unordered_map<std::string, Texture*> m_textureMap;
	};
}

#endif // _CE_TEXTURE_MANAGER_H_
