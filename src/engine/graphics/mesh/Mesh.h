#ifndef _CE_MESH_H_
#define _CE_MESH_H_

#include "Vertex.h"

#include <vector>

namespace CE
{
	struct Mesh
	{
		std::vector<Vertex1P1UV4J> m_vertices;
		std::vector<unsigned int> m_indices;

		std::string m_diffuseMapName;
		std::string m_specularMapName;
		std::string m_normalMapName;
	};

	typedef std::vector<Mesh> Meshes;
}

#endif // _CE_MESH_H_