#ifndef _CE_MESH_DATA_H_
#define _CE_MESH_DATA_H_

#include <Windows.h>

#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

namespace fbxsdk
{
	class FbxManager;
	class FbxMesh;
	class FbxNode;
	class FbxSurfaceMaterial;
}

namespace CE
{
	struct Position
	{
		float x, y, z;
	};

	inline bool operator==(const Position& lhs, const Position& rhs)
	{
		return lhs.x == rhs.x
			&& lhs.y == rhs.y
			&& lhs.z == rhs.z;
	}

	struct TextureCoordinate
	{
		float u, v;
	};

	inline bool operator==(const TextureCoordinate& lhs, const TextureCoordinate& rhs)
	{
		return lhs.u == rhs.u
			&& lhs.v == rhs.v;
	}

	struct Vertex
	{
		Position position;
		TextureCoordinate textureCoordinate;
	};

	inline bool operator==(const Vertex& lhs, const Vertex& rhs)
	{
		return lhs.position == rhs.position
			&& lhs.textureCoordinate == rhs.textureCoordinate;
	}

	struct Joint
	{
		glm::mat4 inverseBindPose;
		std::string name;
		byte parentIndex;
	};

	struct Skeleton
	{
		int jointCount;
		std::vector<Joint> joints;
	};

	struct JointPose
	{
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 scale;
	};

	struct SkeletonPose
	{
		Skeleton* skeleton;
		std::vector<JointPose> localPoses;
	};

	struct MeshData
	{
	public:
		// Loads in all mesh data from the specified .fbx file.
		MeshData();
		// Releases all mesh data.
		~MeshData();

		bool LoadMesh(fbxsdk::FbxManager* fbxManager, const char* szFileName);

		void Draw();

	private:
		void ParseNodes(fbxsdk::FbxNode* pFbxRootNode);
		void LoadInformation(fbxsdk::FbxMesh* pMesh);
		void ProcessMaterialTexture(fbxsdk::FbxSurfaceMaterial* inMaterial);
		void ProcessAnimation(fbxsdk::FbxNode* node);
		void ProcessSkeletonHierarchy(FbxNode* inRootNode);
		void ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int inDepth, int myIndex, int inParentIndex);

	//private:
	public:

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;

		std::string m_diffuseMapName;
		std::string m_specularMapName;
		std::string m_normalMapName;

		Skeleton m_skeleton;
	};
}

#endif // _CE_MESH_DATA_H_