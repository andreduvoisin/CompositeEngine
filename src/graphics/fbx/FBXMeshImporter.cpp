#include "FBXMeshImporter.h"

#include "FBXUtilities.h"
#include "graphics\Mesh.h"
#include "graphics\Skeleton.h"

#include <fbxsdk.h>

namespace CE
{
	FBXMeshImporter::FBXMeshImporter(
			FbxManager* fbxManager,
			const char* szFileName,
			const Skeleton& skeleton,
			Meshes* outMeshes)
		: m_fbxManager(fbxManager)
		, m_szFileName(szFileName)
		, m_skeleton(skeleton)
		, m_outMeshes(outMeshes)
	{

	}

	bool FBXMeshImporter::LoadMeshes()
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

		ParseNodes(pFbxRootNode, pFbxScene);

		return true;
	}

	void FBXMeshImporter::ParseNodes(FbxNode* pFbxRootNode, FbxScene* pFbxScene)
	{
		PrintNode(pFbxRootNode);

		const int meshCount = pFbxScene->GetSrcObjectCount<FbxMesh>();
		m_outMeshes->resize(meshCount);

		for (int i = 0; i < meshCount; i++)
		{
			FbxMesh* pMesh = pFbxScene->GetSrcObject<FbxMesh>(i);
			FbxNode* pNode = pMesh->GetNode();

			Mesh& currentMesh = m_outMeshes->at(i);
			m_controlPointToVertices.clear();

			unsigned int materialCount = pNode->GetMaterialCount();
			for (unsigned int i = 0; i < materialCount; ++i)
			{
				FbxSurfaceMaterial* surfaceMaterial = pNode->GetMaterial(i);
				ProcessMaterialTexture(currentMesh, surfaceMaterial);
			}

			ProcessVertices(currentMesh, pMesh);
			ProcessSkinnedMesh(currentMesh, pNode, pFbxScene);

			printf("currentMesh.m_diffuseMapName: %s\n", currentMesh.m_diffuseMapName.c_str());
			printf("currentMesh.m_specularMapName: %s\n", currentMesh.m_specularMapName.c_str());
			printf("currentMesh.m_normalMapName: %s\n", currentMesh.m_normalMapName.c_str());
		}
	}

	void FBXMeshImporter::ProcessVertices(Mesh& currentMesh, FbxMesh* pMesh)
	{
		//get all UV set names
		FbxStringList lUVSetNameList;
		pMesh->GetUVSetNames(lUVSetNameList);

		for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
		{
			const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
			const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);
			printf("lUVSetName: %s\n", lUVSetName);
		}

		FbxVector4* pVertices = pMesh->GetControlPoints();
		currentMesh.m_indices.reserve(pMesh->GetControlPointsCount());

		int quadCount = 0, triCount = 0, unknownCount = 0;

		//iterating over all uv sets
		//for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
		//{
		//get lUVSetIndex-th uv set
		//const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
		//const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);
		const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(0);// , fbxsdk::FbxLayerElement::eTextureDiffuse);

		if (!lUVElement)
		{
			//continue;
			return;
		}

		// only support mapping mode eByPolygonVertex and eByControlPoint
		if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex
			&& lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
		{
			//continue;
			return;
		}

		//index array, where holds the index referenced to the uv data
		const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

		//iterating through the data by polygon
		const int lPolyCount = pMesh->GetPolygonCount();
		//printf("lUVSetName: %s\n", lUVSetName);
		printf("lPolyCount: %i\n", lPolyCount);

		const FbxAMatrix geometryTransform = pMesh->GetNode()->EvaluateGlobalTransform();

		int lPolyIndexCounter = 0;
		for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
		{
			// build the max index array that we need to pass into MakePoly
			const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
			if (lPolySize < 3)
			{
				printf("degenerate polygon!\n");
				continue;
			}

			const int numTris = lPolySize - 2;

			for (int tris = 0; tris < numTris; ++tris)
			{
				for (int lVertIndex = 0; lVertIndex < 3; ++lVertIndex)
				{
					int currentIndex = lVertIndex;
					if (currentIndex != 0)
					{
						currentIndex += tris;
					}

					//get the index of the current vertex in control points array
					int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, currentIndex);
					
					FbxVector4 vertexPosition(
						pVertices[lPolyVertIndex][0],
						pVertices[lPolyVertIndex][1],
						pVertices[lPolyVertIndex][2],
						1.f);
					vertexPosition = geometryTransform.MultT(vertexPosition);

					Vertex vertex;
					vertex.numWeights = 0;
					vertex.position.x = (float)vertexPosition[0];
					vertex.position.y = (float)vertexPosition[1];
					vertex.position.z = (float)vertexPosition[2];

					FbxVector2 lUVValue;

					//the UV index depends on the reference mode
					int lUVIndex = 0;
					if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
					{
						lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;
					}
					else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						int adjustedIndexCounter = lPolyIndexCounter + currentIndex;
						if (adjustedIndexCounter < lIndexCount)
						{
							//the UV index depends on the reference mode
							lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(adjustedIndexCounter) : adjustedIndexCounter;
						}
					}

					lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

					vertex.textureCoordinate.u = lUVValue[0];
					vertex.textureCoordinate.v = lUVValue[1];

					unsigned int index;
					for (index = 0; index < currentMesh.m_vertices.size(); ++index)
					{
						if (vertex == currentMesh.m_vertices[index])
						{
							break;
						}
					}

					if (index == currentMesh.m_vertices.size())
					{
						currentMesh.m_vertices.push_back(vertex);
						m_controlPointToVertices[lPolyVertIndex].push_back(currentMesh.m_vertices.size() - 1);
					}

					currentMesh.m_indices.push_back(index);
				}
			}

			lPolyIndexCounter += lPolySize;
		}
		//}
	}

	void FBXMeshImporter::ProcessMaterialTexture(Mesh& currentMesh, fbxsdk::FbxSurfaceMaterial* inMaterial)
	{
		unsigned int textureIndex = 0;
		fbxsdk::FbxProperty property;

		FBXSDK_FOR_EACH_TEXTURE(textureIndex)
		{
			property = inMaterial->FindProperty(fbxsdk::FbxLayerElement::sTextureChannelNames[textureIndex]);
			if (property.IsValid())
			{
				unsigned int textureCount = property.GetSrcObjectCount<fbxsdk::FbxTexture>();
				printf("textureCount: %u\n", textureCount);
				for (unsigned int i = 0; i < textureCount; ++i)
				{
					fbxsdk::FbxLayeredTexture* layeredTexture = property.GetSrcObject<fbxsdk::FbxLayeredTexture>(i);
					if (layeredTexture)
					{
						printf("Layered Texture is currently unsupported\n");
						continue;
					}
					else
					{
						fbxsdk::FbxTexture* texture = property.GetSrcObject<fbxsdk::FbxTexture>(i);
						printf("userdataptr: %s\n", texture->GetUserDataPtr());
						if (texture)
						{
							std::string textureType = property.GetNameAsCStr();
							fbxsdk::FbxFileTexture* fileTexture = FbxCast<fbxsdk::FbxFileTexture>(texture);

							if (fileTexture)
							{
								printf("textureType: %s\n", textureType.c_str());
								if (textureType == "DiffuseColor")
								{
									currentMesh.m_diffuseMapName = fileTexture->GetFileName();
								}
								else if (textureType == "SpecularColor")
								{
									currentMesh.m_specularMapName = fileTexture->GetFileName();
								}
								else if (textureType == "NormalMap")
								{
									currentMesh.m_normalMapName = fileTexture->GetFileName();
								}
							}
						}
					}
				}
			}
		}
	}

	void FBXMeshImporter::ProcessSkinnedMesh(Mesh& currentMesh, FbxNode* node, FbxScene* scene)
	{
		FbxMesh* currMesh = node->GetMesh();
		unsigned int numOfDeformers = currMesh->GetDeformerCount();
		FbxAMatrix geometryTransform = FbxAMatrix(
			node->GetGeometricTranslation(FbxNode::eSourcePivot),
			node->GetGeometricRotation(FbxNode::eSourcePivot),
			node->GetGeometricScaling(FbxNode::eSourcePivot));

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
				for (unsigned i = 0; i < m_skeleton.joints.size(); ++i)
				{
					if (m_skeleton.joints[i].name == currJointName)
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

				unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
				for (unsigned controlPointIndex = 0; controlPointIndex < numOfIndices; ++controlPointIndex)
				{
					const auto& verticesForControlPoint = m_controlPointToVertices[currCluster->GetControlPointIndices()[controlPointIndex]];
					for (unsigned vertexIndex = 0; vertexIndex < verticesForControlPoint.size(); ++vertexIndex)
					{
						Vertex& vertex = currentMesh.m_vertices[verticesForControlPoint[vertexIndex]];

						if (vertex.numWeights >= 4)
						{
							// TODO: either take all weights, or take the 4 most weighted, or something better than just the first 4
							printf("too many weights!\n");
							continue;
						}

						vertex.jointIndices[vertex.numWeights] = currJointIndex;
						vertex.jointWeights[vertex.numWeights] = currCluster->GetControlPointWeights()[controlPointIndex];
						vertex.numWeights++;
					}
				}
			}
		}

		// ensure we have 4 joints per vertex
		for (auto it = currentMesh.m_vertices.begin(); it != currentMesh.m_vertices.end(); ++it)
		{
			while (it->numWeights < 4)
			{
				it->jointIndices[it->numWeights] = 0;
				it->jointWeights[it->numWeights] = 0;
				it->numWeights++;
			}
		}
	}
}
