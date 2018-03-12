#include "MeshManager.h"

#include "Mesh.h"
#include "graphics\File3DImporter.h"

#include <string>

namespace CE
{
	void MeshManager::Initialize(File3DImporter* importer)
	{
		m_importer = importer;
	}

	void MeshManager::Destroy()
	{
		for (auto it = m_meshesMap.begin(); it != m_meshesMap.end(); ++it)
		{
			delete it->second;
		}
		m_meshesMap.clear();
	}

	Meshes* MeshManager::GetMeshes(const char* szMeshFile, const Skeleton& skeleton)
	{
		// Try to find the mesh.
		auto it = m_meshesMap.find(szMeshFile);

		// If it exists, return it.
		if (it != m_meshesMap.end())
		{
			return it->second;
		}

		// Otherwise, attempt to load it.
		Meshes* meshes = new Meshes();
		if (!m_importer->ExtractMeshes(szMeshFile, skeleton, meshes))
		{
			delete meshes;
			return nullptr;
		}
		m_meshesMap[szMeshFile] = meshes;
		return meshes;
	}
}
