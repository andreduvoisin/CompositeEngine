#include "FBXSkeletonImporter.h"

#include "FBXValidator.h"
#include "graphics/skeleton/Skeleton.h"

#include <fbxsdk.h>

namespace CE
{
	FBXSkeletonImporter::FBXSkeletonImporter(
			FbxManager* fbxManager,
			const char* fileName,
			Skeleton* outSkeleton)
		: fbxManager(fbxManager)
		, fileName(fileName)
		, outSkeleton(outSkeleton)
	{

	}

	bool FBXSkeletonImporter::LoadSkeleton()
	{
		FBXValidator validator(fbxManager, fileName);
		if (!validator.Validate())
		{
			return false;
		}

		FbxImporter* importer = FbxImporter::Create(fbxManager, "");
		FbxScene* scene = FbxScene::Create(fbxManager, "");

		if (!importer->Initialize(fileName, -1, fbxManager->GetIOSettings()))
		{
			return false;
		}

		if (!importer->Import(scene))
		{
			return false;
		}

		importer->Destroy();

		FbxNode* rootNode = scene->GetRootNode();
		if (!rootNode)
		{
			return false;
		}

		ProcessSkeletonHierarchy(rootNode, scene);

		return true;
	}

	void FBXSkeletonImporter::ProcessSkeletonHierarchy(FbxNode* rootNode, FbxScene* scene)
	{
		for (int childIndex = 0; childIndex < rootNode->GetChildCount(); ++childIndex)
		{
			FbxNode* node = rootNode->GetChild(childIndex);
			ProcessSkeletonHierarchyRecursively(node, 0, -1);
		}


		std::vector<bool> isJointUsed;
		isJointUsed.resize(outSkeleton->joints.size(), false);

		const int meshCount = scene->GetSrcObjectCount<FbxMesh>();
		for (int meshIndex = 0; meshIndex < meshCount; meshIndex++)
		{
			FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(meshIndex);

			unsigned int deformerCount = mesh->GetDeformerCount();

			for (unsigned deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
			{
				FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
				if (!skin)
				{
					continue;
				}

				unsigned clusterCount = skin->GetClusterCount();
				for (unsigned clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
				{
					FbxCluster* cluster = skin->GetCluster(clusterIndex);
					std::string jointName = cluster->GetLink()->GetName();

					unsigned int jointIndex = -1;
					for (unsigned i = 0; i < outSkeleton->joints.size(); ++i)
					{
						if (outSkeleton->joints[i].name == jointName)
						{
							jointIndex = i;
							break;
						}
					}

					// TODO: Swap to ozz-animation way of doing this?
					if (jointIndex == -1)
					{
						printf("Couldn't find joint index.\n");
						continue; // should this break? or return even?
					}

					// TransformLink refers to global initial transform of the link node.
					// TransformLink is the global transform of the bone(link) at the binding moment.
					// In the future, if local transform is needed (instead of global), you must multiply up the chain with inverses.
					FbxAMatrix transformLinkMatrix;
					transformLinkMatrix = cluster->GetTransformLinkMatrix(transformLinkMatrix);

					const FbxAMatrix inverseBindPose = transformLinkMatrix.Inverse();

					for (unsigned i = 0; i < 16; ++i)
					{
						outSkeleton->joints[jointIndex].inverseBindPose[i / 4][i % 4] = (float) inverseBindPose.Get(i / 4, i % 4);
					}

					unsigned int indexCount = cluster->GetControlPointIndicesCount();
					if (indexCount > 0)
					{
						isJointUsed[jointIndex] = true;
					}
				}
			}
		}

		size_t jointsRemoved = 0;
		for (size_t i = 0; i < isJointUsed.size(); ++i)
		{
			if (!isJointUsed[i] && !JointHasChild(i - jointsRemoved))
			{
				RemoveJoint(i - jointsRemoved);
				++jointsRemoved;
			}
		}
	}

	bool FBXSkeletonImporter::JointHasChild(size_t index)
	{
		for (size_t i = 0; i < outSkeleton->joints.size(); ++i)
		{
			if (outSkeleton->joints[i].parentIndex == index)
			{
				return true;
			}
		}

		return false;
	}

	void FBXSkeletonImporter::RemoveJoint(size_t index)
	{
		outSkeleton->joints.erase(outSkeleton->joints.begin() + index);

		for (size_t i = index; i < outSkeleton->joints.size(); ++i)
		{
			if (outSkeleton->joints[i].parentIndex > static_cast<short>(index))
			{
				--outSkeleton->joints[i].parentIndex;
			}
		}
	}

	void FBXSkeletonImporter::ProcessSkeletonHierarchyRecursively(FbxNode* node, size_t index, size_t parentIndex)
	{
		if (node->GetNodeAttribute()
			&& node->GetNodeAttribute()->GetAttributeType()
			&& node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			Joint joint;
			joint.parentIndex = (short) parentIndex;
			joint.name = node->GetName();
			joint.inverseBindPose = glm::mat4(1.f);

			outSkeleton->joints.push_back(joint);
		}

		for (int i = 0; i < node->GetChildCount(); i++)
		{
			ProcessSkeletonHierarchyRecursively(node->GetChild(i), outSkeleton->joints.size(), index);
		}
	}
}
