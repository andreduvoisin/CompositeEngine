#ifndef _CE_MESH_DATA_H_
#define _CE_MESH_DATA_H_

#include <Windows.h>

#include <vector>
#include <unordered_map>
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

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
		int jointIndices[4];
		float jointWeights[4];
		unsigned numWeights;
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
		short parentIndex;
	};

	struct Skeleton
	{
		std::vector<Joint> joints;
	};

	struct KeyFrame
	{
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;

		float time;

		KeyFrame()
			: translation(glm::vec3())
			, rotation(glm::vec3())
			, scale(glm::vec3())
			, time(0.f)
		{

		}
	};

	struct Animation
	{
		std::string name;
		std::vector<std::vector<KeyFrame>> keyFrames;
		float currTime;
		float animationTime;
	};

	struct MeshData
	{
	public:
		// Loads in all mesh data from the specified .fbx file.
		MeshData();
		// Releases all mesh data.
		~MeshData();

		bool LoadMesh(fbxsdk::FbxManager* fbxManager, const char* szFileName);

		void InitializeAnimationData();
		//void CalculatePose(short joint, KeyFrame* frame1, KeyFrame* frame2);

		void Draw();
		void Update(float deltaTime);

	private:
		void ParseNodes(fbxsdk::FbxNode* pFbxRootNode, fbxsdk::FbxScene* pFbxScene);
		void LoadInformation(fbxsdk::FbxMesh* pMesh);
		void ProcessMaterialTexture(fbxsdk::FbxSurfaceMaterial* inMaterial);
		void ProcessAnimation(fbxsdk::FbxNode* node, fbxsdk::FbxScene* scene);
		void ProcessSkeletonHierarchy(fbxsdk::FbxNode* inRootNode);
		void ProcessSkeletonHierarchyRecursively(fbxsdk::FbxNode* inNode, int inDepth, int myIndex, int inParentIndex);

		glm::mat4 ToAffineMatrix(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale);
		glm::mat4 GetLocalPose(const KeyFrame& low, const KeyFrame& high, float alpha);
		glm::vec3 LerpTranslation(const glm::vec3& low, const glm::vec3& high, float alpha);
		glm::quat LerpRotation(const glm::quat& low, const glm::quat& high, float alpha);
		glm::vec3 LerpScale(const glm::vec3& low, const glm::vec3& high, float alpha);
		glm::vec3 Vec3Lerp(const glm::vec3& a, const glm::vec3& b, float alpha);

	//private:
	public:

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;

		std::string m_diffuseMapName;
		std::string m_specularMapName;
		std::string m_normalMapName;

		std::unordered_map<int, std::vector<int>> m_controlPointToVertices;

		// what even is an animation component lol
		Skeleton m_skeleton;
		Animation m_animation;
		std::vector<glm::mat4> m_palette;
	};
}

#endif // _CE_MESH_DATA_H_