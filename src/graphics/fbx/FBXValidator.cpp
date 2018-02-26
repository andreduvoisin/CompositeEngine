#include "FBXValidator.h"

#include <fbxsdk.h>

namespace CE
{
	FBXValidator::FBXValidator(
			FbxManager* manager, 
			const char* fileName)
		: manager(manager)
		, fileName(fileName)
	{
		
	}

	bool FBXValidator::Validate()
	{
		FbxImporter* importer = FbxImporter::Create(manager, "");
		FbxScene* scene = FbxScene::Create(manager, "");

		if (!importer->Initialize(fileName, -1, manager->GetIOSettings()))
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
			&& ValidateChildren(node);
	}

	bool FBXValidator::ValidateGeometry(FbxNode* node)
	{
		FbxAMatrix geometryTransform(
			node->GetGeometricTranslation(FbxNode::eSourcePivot),
			node->GetGeometricRotation(FbxNode::eSourcePivot),
			node->GetGeometricScaling(FbxNode::eSourcePivot));

		return geometryTransform.IsIdentity();
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
