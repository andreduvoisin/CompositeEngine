#ifndef _CE_MESH_DATA_H_
#define _CE_MESH_DATA_H_

#include <Windows.h>

#include <vector>


namespace fbxsdk { class FbxManager; }

namespace CE
{
	struct MeshData
	{
	public:
		// Loads in all mesh data from the specified .fbx file.
		MeshData();
		// Releases all mesh data.
		~MeshData();

		bool LoadMesh(fbxsdk::FbxManager* fbxManager, const char* szFileName);

		void Draw();

	//private:
		struct Vertex
		{
			float x, y, z;
		};
		std::vector<Vertex> m_vertices;
		std::vector<WORD> m_indices;
	};
}

#endif // _CE_MESH_DATA_H_