#include "FBXValidator.h"

#include <fbxsdk.h>

namespace CE
{
	FBXValidator::FBXValidator(
			FbxManager* fbxManager, 
			const char* fileName)
		: fbxManager(fbxManager)
		, fileName(fileName)
	{
		
	}

	bool FBXValidator::Validate()
	{
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

		return ValidateNode(rootNode);
	}

	bool FBXValidator::ValidateNode(FbxNode* node)
	{
		return ValidateGeometry(node)
			&& ValidateCluster(node)
			&& ValidateChildren(node);
	}

	bool FBXValidator::ValidateGeometry(FbxNode* node)
	{
		FbxAMatrix geometryTransform(
			node->GetGeometricTranslation(FbxNode::EPivotSet::eSourcePivot),
			node->GetGeometricRotation(FbxNode::EPivotSet::eSourcePivot),
			node->GetGeometricScaling(FbxNode::EPivotSet::eSourcePivot));

		return geometryTransform.IsIdentity();
	}

	// TODO: Validate the whole skin. (see ozz-animation)
	bool FBXValidator::ValidateCluster(fbxsdk::FbxNode* node)
	{
		FbxMesh* mesh = node->GetMesh();
		if (!mesh)
		{
			return true;
		}

		const unsigned deformerCount = mesh->GetDeformerCount();
		for (unsigned deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
		{
			FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
			if (!skin)
			{
				continue;
			}

			const unsigned clusterCount = skin->GetClusterCount();
			for (unsigned clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
			{
				FbxCluster* cluster = skin->GetCluster(clusterIndex);
				const FbxCluster::ELinkMode linkMode = cluster->GetLinkMode();
				if (linkMode != FbxCluster::ELinkMode::eNormalize)
				{
					return false;
				}
			}
		}

		return true;
	}


	bool FBXValidator::ValidateChildren(fbxsdk::FbxNode* node)
	{
		for (int i = 0; i < node->GetChildCount(); ++i)
		{
			FbxNode* childNode = node->GetChild(i);
			if (!ValidateNode(childNode))
			{
				return false;
			}
		}

		return true;
	}
}
