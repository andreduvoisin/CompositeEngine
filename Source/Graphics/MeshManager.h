#ifndef _CE_MESH_MANAGER_H_
#define _CE_MESH_MANAGER_H_

#include "Common\Singleton.h"

#include <unordered_map>


namespace fbxsdk { class FbxManager; }

namespace CE
{
	struct MeshData;

	class MeshManager : public Singleton<MeshManager>
	{
	public:
		void Initialize();
		void Destroy();

		MeshData* GetMeshData(const char* szMeshFile);

	private:
		std::unordered_map<std::string, MeshData*> m_meshMap;
		
		fbxsdk::FbxManager* g_pFbxSdkManager;
	};
}

#endif // _CE_MESH_MANAGER_H_