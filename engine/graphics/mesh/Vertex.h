#ifndef _CE_VERTEX_H_
#define _CE_VERTEX_H_

#include <glm/glm.hpp>

namespace CE
{
	struct Vertex1P1UV
	{
		glm::vec3 position;
		float uv[2];
	};

	inline bool operator==(const Vertex1P1UV& lhs, const Vertex1P1UV& rhs)
	{
		return lhs.position == rhs.position
			&& lhs.uv[0] == rhs.uv[0]
			&& lhs.uv[1] == rhs.uv[1];
	}

	struct Vertex1P1UV4J
	{
		glm::vec3 position;
		float uv[2];
		uint8_t jointIndices[4];
		float jointWeights[3];
	};

	inline bool operator==(const Vertex1P1UV4J& lhs, const Vertex1P1UV4J& rhs)
	{
		return lhs.position == rhs.position
			&& lhs.uv[0] == rhs.uv[0]
			&& lhs.uv[1] == rhs.uv[1]
			&& lhs.jointIndices[0] == rhs.jointIndices[0]
			&& lhs.jointIndices[1] == rhs.jointIndices[1]
			&& lhs.jointIndices[2] == rhs.jointIndices[2]
			&& lhs.jointIndices[3] == rhs.jointIndices[3]
			&& lhs.jointWeights[0] == rhs.jointWeights[0]
			&& lhs.jointWeights[1] == rhs.jointWeights[1]
			&& lhs.jointWeights[2] == rhs.jointWeights[2];
	}
}

#endif // _CE_VERTEX_H_
