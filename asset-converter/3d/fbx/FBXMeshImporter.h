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
			const char* fileName,
			const Skeleton& skeleton,
			Meshes* outMeshes);

		bool LoadMeshes();

	private:
		void ParseNodes(fbxsdk::FbxNode* rootNode, fbxsdk::FbxScene* scene);
		void ProcessVertices(Mesh& currentMesh, fbxsdk::FbxMesh* fbxMesh);
		void ProcessMaterialTexture(Mesh& currentMesh, fbxsdk::FbxSurfaceMaterial* material);
		void ProcessSkinnedMesh(Mesh& currentMesh, fbxsdk::FbxNode* node, fbxsdk::FbxScene* scene);

	private:
		fbxsdk::FbxManager* fbxManager;
		const char* fileName;
		const Skeleton& skeleton;
		Meshes* outMeshes;

		std::unordered_map<int, std::vector<int>> controlPointToVertices;
	};
}

#endif // _CE_FBX_MESH_IMPORTER_H_