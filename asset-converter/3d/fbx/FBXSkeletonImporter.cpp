#include "FBXSkeletonImporter.h"

#include "FBXValidator.h"
#include "graphics/skeleton/Skeleton.h"

#include <fbxsdk.h>

namespace CE
{
	FBXSkeletonImporter::FBXSkeletonImporter(
			FbxManager* fbxManager,
			const char* szFileName,
			Skeleton* outSkeleton)
		: m_fbxManager(fbxManager)
		, m_szFileName(szFileName)
		, m_outSkeleton(outSkeleton)
	{

	}

	bool FBXSkeletonImporter::LoadSkeleton()
	{
		FBXValidator validator(m_fbxManager, m_szFileName);
		if (!validator.Validate())
		{
			return false;
		}

		FbxImporter* pImporter = FbxImporter::Create(m_fbxManager, "");
		FbxScene* pFbxScene = FbxScene::Create(m_fbxManager, "");

		if (!pImporter->Initialize(m_szFileName, -1, m_fbxManager->GetIOSettings()))
		{
			return false;
		}

		if (!pImporter->Import(pFbxScene))
		{
			return false;
		}

		pImporter->Destroy();

		FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
		if (!pFbxRootNode)
		{
			return false;
		}

		ProcessSkeletonHierarchy(pFbxRootNode, pFbxScene);

		return true;
	}

	void FBXSkeletonImporter::ProcessSkeletonHierarchy(FbxNode* inRootNode, FbxScene* pFbxScene)
	{
		for (int childIndex = 0; childIndex < inRootNode->GetChildCount(); ++childIndex)
		{
			FbxNode* currNode = inRootNode->GetChild(childIndex);
			ProcessSkeletonHierarchyRecursively(currNode, 0, -1);
		}


		std::vector<bool> isJointUsed;
		isJointUsed.resize(m_outSkeleton->joints.size(), false);

		const int meshCount = pFbxScene->GetSrcObjectCount<FbxMesh>();
		for (int meshIndex = 0; meshIndex < meshCount; meshIndex++)
		{
			FbxMesh* currMesh = pFbxScene->GetSrcObject<FbxMesh>(meshIndex);

			unsigned int numOfDeformers = currMesh->GetDeformerCount();

			for (unsigned deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
			{
				FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
				if (!currSkin)
				{
					continue;
				}

				unsigned numOfClusters = currSkin->GetClusterCount();
				for (unsigned clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
				{
					FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
					std::string currJointName = currCluster->GetLink()->GetName();

					unsigned int currJointIndex = -1;
					for (unsigned i = 0; i < m_outSkeleton->joints.size(); ++i)
					{
						if (m_outSkeleton->joints[i].name == currJointName)
						{
							currJointIndex = i;
							break;
						}
					}

					// TODO: Swap to ozz-animation way of doing this?
					if (currJointIndex == -1)
					{
						printf("Couldn't find joint index.\n");
						continue; // should this break? or return even?
					}

					// TransformLink refers to global initial transform of the link node.
					// TransformLink is the global transform of the bone(link) at the binding moment.
					// In the future, if local transform is needed (instead of global), you must multiply up the chain with inverses.
					FbxAMatrix transform_link_matrix;
					transform_link_matrix = currCluster->GetTransformLinkMatrix(transform_link_matrix);

					const FbxAMatrix inverse_bind_pose = transform_link_matrix.Inverse();

					for (unsigned i = 0; i < 16; ++i)
					{
						m_outSkeleton->joints[currJointIndex].inverseBindPose[i / 4][i % 4] = (float) inverse_bind_pose.Get(i / 4, i % 4);
					}

					unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
					if (numOfIndices > 0)
					{
						isJointUsed[currJointIndex] = true;
					}
				}
			}
		}

		size_t numJointsRemoved = 0;
		for (size_t i = 0; i < isJointUsed.size(); ++i)
		{
			if (!isJointUsed[i] && !JointHasChild(i - numJointsRemoved))
			{
				RemoveJoint(i - numJointsRemoved);
				++numJointsRemoved;
			}
		}
	}

	bool FBXSkeletonImporter::JointHasChild(size_t index)
	{
		for (size_t i = 0; i < m_outSkeleton->joints.size(); ++i)
		{
			if (m_outSkeleton->joints[i].parentIndex == index)
			{
				return true;
			}
		}

		return false;
	}

	void FBXSkeletonImporter::RemoveJoint(size_t index)
	{
		m_outSkeleton->joints.erase(m_outSkeleton->joints.begin() + index);

		for (size_t i = index; i < m_outSkeleton->joints.size(); ++i)
		{
			if (m_outSkeleton->joints[i].parentIndex > static_cast<short>(index))
			{
				--m_outSkeleton->joints[i].parentIndex;
			}
		}
	}

	void FBXSkeletonImporter::ProcessSkeletonHierarchyRecursively(FbxNode* inNode, size_t myIndex, size_t inParentIndex)
	{
		if (inNode->GetNodeAttribute()
			&& inNode->GetNodeAttribute()->GetAttributeType()
			&& inNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			Joint currJoint;
			currJoint.parentIndex = (short) inParentIndex;
			currJoint.name = inNode->GetName();
			currJoint.inverseBindPose = glm::mat4(1.f);

			m_outSkeleton->joints.push_back(currJoint);
		}
		for (int i = 0; i < inNode->GetChildCount(); i++)
		{
			ProcessSkeletonHierarchyRecursively(inNode->GetChild(i), m_outSkeleton->joints.size(), myIndex);
		}
	}
}
