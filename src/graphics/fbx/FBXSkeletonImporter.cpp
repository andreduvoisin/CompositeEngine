#include "FBXSkeletonImporter.h"

#include "graphics\Skeleton.h"

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

		ProcessSkeletonHierarchy(pFbxRootNode);

		return true;
	}

	void FBXSkeletonImporter::ProcessSkeletonHierarchy(FbxNode* inRootNode)
	{
		for (int childIndex = 0; childIndex < inRootNode->GetChildCount(); ++childIndex)
		{
			FbxNode* currNode = inRootNode->GetChild(childIndex);
			ProcessSkeletonHierarchyRecursively(currNode, 0, -1);
		}


		//for (int i = 1; i < m_outSkeleton->joints.size(); ++i)
		//{
		//	m_outSkeleton->joints[i].inverseBindPose = m_outSkeleton->joints[m_outSkeleton->joints[i].parentIndex].inverseBindPose * m_outSkeleton->joints[i].inverseBindPose;
		//}

		//for (int i = 0; i < m_outSkeleton->joints.size(); ++i)
		//{
		//	// TODO: there's a faster but less precise matrix inverse for affine matricies, if this is going to be done at runtime (but i don't think it will)
		//	m_outSkeleton->joints[i].inverseBindPose = glm::inverse(m_outSkeleton->joints[i].inverseBindPose);
		//}


		for (unsigned i = 0; i < m_outSkeleton->joints.size(); ++i)
		{
			m_outSkeleton->joints[i].inverseBindPose = glm::mat4(1.f);
		}
	}

	void FBXSkeletonImporter::ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int myIndex, int inParentIndex)
	{
		if (inNode->GetNodeAttribute()
			&& inNode->GetNodeAttribute()->GetAttributeType()
			&& inNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			Joint currJoint;
			currJoint.parentIndex = inParentIndex;
			currJoint.name = inNode->GetName();

			FbxAMatrix bindPose = inParentIndex == -1 ? inNode->EvaluateGlobalTransform() : inNode->EvaluateLocalTransform();
			for (unsigned i = 0; i < 16; ++i)
			{
				currJoint.inverseBindPose[i / 4][i % 4] = bindPose.Get(i / 4, i % 4);
			}

			m_outSkeleton->joints.push_back(currJoint);
		}
		for (int i = 0; i < inNode->GetChildCount(); i++)
		{
			ProcessSkeletonHierarchyRecursively(inNode->GetChild(i), m_outSkeleton->joints.size(), myIndex);
		}
	}
}
