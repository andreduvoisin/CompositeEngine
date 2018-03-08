#include "FBXMeshImporter.h"

#include "FBXUtilities.h"
#include "FBXValidator.h"
#include "graphics\mesh\Mesh.h"
#include "graphics\skeleton\Skeleton.h"

#include <fbxsdk.h>
#include <algorithm>

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

		const FbxAMatrix globalTransform = pMesh->GetNode()->EvaluateGlobalTransform();

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
					
					FbxVector4 vertexPosition = pMesh->GetControlPointAt(lPolyVertIndex);
					vertexPosition[3] = 1.f;
					vertexPosition = globalTransform.MultT(vertexPosition);

					Vertex1P1UV4J vertex;
					vertex.position.x = static_cast<float>(vertexPosition[0]);
					vertex.position.y = static_cast<float>(vertexPosition[1]);
					vertex.position.z = static_cast<float>(vertexPosition[2]);

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

					vertex.uv[0] = lUVValue[0];
					vertex.uv[1] = lUVValue[1];

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

		struct JointIndexWeightPair
		{
			unsigned index;
			float weight;
		};
		std::vector<std::vector<JointIndexWeightPair>> jointIndexWeightPairs;
		jointIndexWeightPairs.resize(currentMesh.m_vertices.size());

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

				if (currJointIndex == -1)
				{
					continue;
				}

				unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
				for (unsigned controlPointIndex = 0; controlPointIndex < numOfIndices; ++controlPointIndex)
				{
					const auto& verticesForControlPoint = m_controlPointToVertices[currCluster->GetControlPointIndices()[controlPointIndex]];
					for (unsigned vertexIndex = 0; vertexIndex < verticesForControlPoint.size(); ++vertexIndex)
					{
						JointIndexWeightPair jointIndexWeightPair;
						jointIndexWeightPair.index = currJointIndex;
						jointIndexWeightPair.weight = currCluster->GetControlPointWeights()[controlPointIndex];
						jointIndexWeightPairs[verticesForControlPoint[vertexIndex]].push_back(jointIndexWeightPair);
					}
				}
			}
		}

		for (size_t i = 0; i < currentMesh.m_vertices.size(); ++i)
		{
			std::sort(jointIndexWeightPairs[i].begin(), jointIndexWeightPairs[i].end(), 
				[](const JointIndexWeightPair& lhs, const JointIndexWeightPair& rhs) -> bool {
					return lhs.weight > rhs.weight;
				});

			const unsigned maxWeights = 4;
			unsigned numWeights = jointIndexWeightPairs[i].size();

			if (numWeights > maxWeights)
			{
				printf("WARNING: %u Weights, %u Max\n", numWeights, maxWeights);
				numWeights = maxWeights;
			}

			float sum = 0.f;
			for (size_t j = 0; j < numWeights; ++j)
			{
				sum += jointIndexWeightPairs[i][j].weight;
			}
			const float inverseSum = 1.f / (sum == 0.f ? 1.f : sum);
			for (size_t j = 0; j < numWeights; ++j)
			{
				jointIndexWeightPairs[i][j].weight *= inverseSum;
			}

			size_t currentWeight;
			for (currentWeight = 0; currentWeight < numWeights; ++currentWeight)
			{
				currentMesh.m_vertices[i].jointIndices[currentWeight] = jointIndexWeightPairs[i][currentWeight].index;
				if (currentWeight != maxWeights - 1)
				{
					currentMesh.m_vertices[i].jointWeights[currentWeight] = jointIndexWeightPairs[i][currentWeight].weight;
				}
			}

			for (; currentWeight < maxWeights; ++currentWeight)
			{
				currentMesh.m_vertices[i].jointIndices[currentWeight] = 0;
				if (currentWeight != maxWeights - 1)
				{
					currentMesh.m_vertices[i].jointWeights[currentWeight] = 0.f;
				}
			}
		}
	}
}
