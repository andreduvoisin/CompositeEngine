#include "FBXValidator.h"

#include <fbxsdk.h>

namespace CE
{
	FBXValidator::FBXValidator(
			fbxsdk::FbxManager* manager, 
			const char* fileName)
		: manager(manager)
		, fileName(fileName)
	{
		
	}

	bool FBXValidator::Validate()
	{
		fbxsdk::FbxImporter* importer = fbxsdk::FbxImporter::Create(manager, "");
		fbxsdk::FbxScene* scene = fbxsdk::FbxScene::Create(manager, "");

		if (!importer->Initialize(fileName, -1, manager->GetIOSettings()))
		{
			return false;
		}

		if (!importer->Import(scene))
		{
			return false;
		}

		importer->Destroy();

		fbxsdk::FbxNode* rootNode = scene->GetRootNode();
		if (!rootNode)
		{
			return false;
		}

		return ValidateNode(rootNode);
	}

	bool FBXValidator::ValidateNode(fbxsdk::FbxNode* node)
	{
		return ValidateGeometry(node)
			&& ValidateCluster(node)
			&& ValidateChildren(node);
	}

	bool FBXValidator::ValidateGeometry(fbxsdk::FbxNode* node)
	{
		fbxsdk::FbxAMatrix geometryTransform(
			node->GetGeometricTranslation(fbxsdk::FbxNode::EPivotSet::eSourcePivot),
			node->GetGeometricRotation(fbxsdk::FbxNode::EPivotSet::eSourcePivot),
			node->GetGeometricScaling(fbxsdk::FbxNode::EPivotSet::eSourcePivot));

		return geometryTransform.IsIdentity();
	}

	// TODO: Validate the whole skin. (see ozz-animation)
	bool FBXValidator::ValidateCluster(fbxsdk::FbxNode* node)
	{
		fbxsdk::FbxMesh* mesh = node->GetMesh();
		if (!mesh)
		{
			return true;
		}

		const unsigned deformerCount = mesh->GetDeformerCount();
		for (unsigned deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
		{
			fbxsdk::FbxSkin* skin = reinterpret_cast<fbxsdk::FbxSkin*>(mesh->GetDeformer(deformerIndex, fbxsdk::FbxDeformer::eSkin));
			if (!skin)
			{
				continue;
			}

			const unsigned clusterCount = skin->GetClusterCount();
			for (unsigned clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
			{
				fbxsdk::FbxCluster* cluster = skin->GetCluster(clusterIndex);
				const fbxsdk::FbxCluster::ELinkMode linkMode = cluster->GetLinkMode();
				if (linkMode != fbxsdk::FbxCluster::ELinkMode::eNormalize)
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
			fbxsdk::FbxNode* childNode = node->GetChild(i);
			if (!ValidateNode(childNode))
			{
				return false;
			}
		}

		return true;
	}
}
