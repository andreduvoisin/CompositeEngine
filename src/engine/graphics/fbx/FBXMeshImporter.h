#ifndef _CE_FBX_MESH_IMPORTER_H_
#define _CE_FBX_MESH_IMPORTER_H_

#include <unordered_map>
#include <vector>

namespace fbxsdk
{
	class FbxManager;
	class FbxMesh;
	class FbxNode;
	class FbxScene;
	class FbxSurfaceMaterial;
}

namespace CE
{
	struct Mesh;
	typedef std::vector<Mesh> Meshes;
	struct Skeleton;

	class FBXMeshImporter
	{
	public:
		FBXMeshImporter(
			fbxsdk::FbxManager* fbxManager,
			const char* szFileName,
			const Skeleton& skeleton,
			Meshes* outMeshes);

		bool LoadMeshes();

	private:
		void ParseNodes(fbxsdk::FbxNode* pFbxRootNode, fbxsdk::FbxScene* pFbxScene);
		void ProcessVertices(Mesh& currentMesh, fbxsdk::FbxMesh* pMesh);
		void ProcessMaterialTexture(Mesh& currentMesh, fbxsdk::FbxSurfaceMaterial* inMaterial);
		void ProcessSkinnedMesh(Mesh& currentMesh, fbxsdk::FbxNode* node, fbxsdk::FbxScene* scene);

	private:
		fbxsdk::FbxManager* m_fbxManager;
		const char* m_szFileName;
		const Skeleton& m_skeleton;
		Meshes* m_outMeshes;

		std::unordered_map<int, std::vector<int>> m_controlPointToVertices;
	};
}

#endif // _CE_FBX_MESH_IMPORTER_H_