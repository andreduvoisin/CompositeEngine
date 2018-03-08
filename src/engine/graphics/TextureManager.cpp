#include "TextureManager.h"

#include "Texture.h"
#include "File2DImporter.h"

#include <string>

namespace CE
{
	void TextureManager::Initialize(File2DImporter* importer)
	{
		m_importer = importer;
	}

	void TextureManager::Destroy()
	{
		for (auto it = m_textureMap.begin(); it != m_textureMap.end(); ++it)
		{
			delete it->second;
		}
		m_textureMap.clear();
	}

	Texture* TextureManager::GetTexture(const char* szTextureFile)
	{
		// Try to find the texture.
		auto it = m_textureMap.find(szTextureFile);

		// If it exists, return it.
		if (it != m_textureMap.end())
		{
			return it->second;
		}

		// Otherwise, attempt to load it.
		Texture* texture = new Texture();
		if (!m_importer->ExtractTexture(szTextureFile, texture))
		{
			delete texture;
			return nullptr;
		}
		m_textureMap[szTextureFile] = texture;
		return texture;
	}
}
