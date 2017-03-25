#include "MeshManager.h"

#include "MeshData.h"

#include <fbxsdk.h>

#include <string.h>


namespace CE
{
	void MeshManager::Initialize()
	{
		g_pFbxSdkManager = fbxsdk::FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(g_pFbxSdkManager, IOSROOT);
		g_pFbxSdkManager->SetIOSettings(pIOsettings);
	}

	void MeshManager::Destroy()
	{
		for (auto it = m_meshMap.begin(); it != m_meshMap.end(); ++it)
		{
			delete it->second;
		}
		m_meshMap.clear();
	}

	MeshData* MeshManager::GetMeshData(const char* szMeshFile)
	{
		// Try to find the mesh.
		auto it = m_meshMap.find(szMeshFile);

		// If it exists, return it.
		if (it != m_meshMap.end())
		{
			return it->second;
		}

		// Otherwise, attempt to load it.
		MeshData* meshData = new MeshData();
		if (!meshData->LoadMesh(g_pFbxSdkManager, szMeshFile))
		{
			delete meshData;
			return nullptr;
		}
		m_meshMap[szMeshFile] = meshData;
		return meshData;
	}
}
