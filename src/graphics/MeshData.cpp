#include "MeshData.h"

#include <fbxsdk.h>

#include <assert.h>


namespace CE
{
	MeshData::MeshData()
	{

	}

	MeshData::~MeshData()
	{

	}

	bool MeshData::LoadMesh(FbxManager* fbxManager, const char* szFileName)
	{
		FbxImporter* pImporter = FbxImporter::Create(fbxManager, "");
		FbxScene* pFbxScene = FbxScene::Create(fbxManager, "");

		if (!pImporter->Initialize(szFileName, -1, fbxManager->GetIOSettings()))
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

		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
			{
				continue;
			}

			if (pFbxChildNode->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eMesh)
			{
				continue;
			}

			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();

			FbxVector4* pVertices = pMesh->GetControlPoints();

			for (int j = 0; j < pMesh->GetPolygonCount(); j++)
			{
				int iNumVertices = pMesh->GetPolygonSize(j);
				assert(iNumVertices == 3);

				for (int k = 0; k < iNumVertices; k++)
				{
					int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

					Vertex vertex;
					vertex.x = (float)pVertices[iControlPointIndex].mData[0];
					vertex.y = (float)pVertices[iControlPointIndex].mData[1];
					vertex.z = (float)pVertices[iControlPointIndex].mData[2];
					m_vertices.push_back(vertex);
					//m_vertices.push_back(vertex);

					m_indices.push_back((WORD)m_indices.size());
				}
			}
		}

		return true;
	}

	void MeshData::Draw()
	{

	}
}
