#ifndef _CE_MESH_MANAGER_H_
#define _CE_MESH_MANAGER_H_

#include "common\Singleton.h"

#include <unordered_map>
#include <vector>

namespace CE
{
	struct Skeleton;
	struct Mesh;
	typedef std::vector<Mesh> Meshes;
	class File3DImporter;

	class MeshManager : public Singleton<MeshManager>
	{
	public:
		void Initialize(File3DImporter* importer);
		void Destroy();

		Meshes* GetMeshes(const char* szMeshFile, Skeleton& skeleton);

	private:
		File3DImporter* m_importer;
		std::unordered_map<std::string, Meshes*> m_meshesMap;
	};
}

#endif // _CE_MESH_MANAGER_H_