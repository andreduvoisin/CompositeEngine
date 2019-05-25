#include "FBXMeshImporter.h"

#include "FBXUtilities.h"
#include "FBXValidator.h"
#include "graphics/mesh/Mesh.h"
#include "graphics/skeleton/Skeleton.h"

#include <fbxsdk.h>
#include <algorithm>

namespace CE
{
	FBXMeshImporter::FBXMeshImporter(
			FbxManager* fbxManager,
			const char* fileName,
			const Skeleton& skeleton,
			Meshes* outMeshes)
		: fbxManager(fbxManager)
		, fileName(fileName)
		, skeleton(skeleton)
		, outMeshes(outMeshes)
	{

	}

	bool FBXMeshImporter::LoadMeshes()
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

		ParseNodes(rootNode, scene);

		return true;
	}

	void FBXMeshImporter::ParseNodes(FbxNode* rootNode, FbxScene* scene)
	{
		PrintNode(rootNode);

		const int meshCount = scene->GetSrcObjectCount<FbxMesh>();
		outMeshes->resize(meshCount);

		for (int i = 0; i < meshCount; i++)
		{
			FbxMesh* fbxMesh = scene->GetSrcObject<FbxMesh>(i);
			FbxNode* node = fbxMesh->GetNode();

			Mesh& mesh = outMeshes->at(i);
			controlPointToVertices.clear();

			unsigned int materialCount = node->GetMaterialCount();
			for (unsigned int i = 0; i < materialCount; ++i)
			{
				FbxSurfaceMaterial* surfaceMaterial = node->GetMaterial(i);
				ProcessMaterialTexture(mesh, surfaceMaterial);
			}

			ProcessVertices(mesh, fbxMesh);
			ProcessSkinnedMesh(mesh, node, scene);

			printf("currentMesh.diffuseFileName: %s\n", mesh.diffuseFileName.c_str());
			printf("currentMesh.specularFileName: %s\n", mesh.specularFileName.c_str());
			printf("currentMesh.normalFileName: %s\n", mesh.normalFileName.c_str());
		}
	}

	void FBXMeshImporter::ProcessVertices(Mesh& currentMesh, FbxMesh* fbxMesh)
	{
		//get all UV set names
		FbxStringList uvSetNames;
		fbxMesh->GetUVSetNames(uvSetNames);

		for (int uvSetIndex = 0; uvSetIndex < uvSetNames.GetCount(); uvSetIndex++)
		{
			const char* uvSetName = uvSetNames.GetStringAt(uvSetIndex);
			const FbxGeometryElementUV* uvElement = fbxMesh->GetElementUV(uvSetName);
			printf("uvSetName: %s\n", uvSetName);
		}

		currentMesh.m_indices.reserve(fbxMesh->GetControlPointsCount());

		int quadCount = 0, triCount = 0, unknownCount = 0;

		//iterating over all uv sets
		//for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
		//{
		//get lUVSetIndex-th uv set
		//const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
		//const FbxGeometryElementUV* lUVElement = fbxMesh->GetElementUV(lUVSetName);
		const FbxGeometryElementUV* uvElement = fbxMesh->GetElementUV(0);// , fbxsdk::FbxLayerElement::eTextureDiffuse);

		if (!uvElement)
		{
			//continue;
			return;
		}

		// only support mapping mode eByPolygonVertex and eByControlPoint
		if (uvElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex
			&& uvElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
		{
			//continue;
			return;
		}

		//index array, where holds the index referenced to the uv data
		const bool shouldUseIndex = uvElement->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int indexCount = (shouldUseIndex) ? uvElement->GetIndexArray().GetCount() : 0;

		//iterating through the data by polygon
		const int polyCount = fbxMesh->GetPolygonCount();
		//printf("lUVSetName: %s\n", lUVSetName);
		printf("polyCount: %i\n", polyCount);

		const FbxAMatrix globalTransform = fbxMesh->GetNode()->EvaluateGlobalTransform();

		int polyIndexCounter = 0;
		for (int polyIndex = 0; polyIndex < polyCount; ++polyIndex)
		{
			// build the max index array that we need to pass into MakePoly
			const int polySize = fbxMesh->GetPolygonSize(polyIndex);
			if (polySize < 3)
			{
				printf("degenerate polygon!\n");
				continue;
			}

			const int trisCount = polySize - 2;

			for (int trisIndex = 0; trisIndex < trisCount; ++trisIndex)
			{
				for (int vertIndex = 0; vertIndex < 3; ++vertIndex)
				{
					int currentIndex = vertIndex;
					if (currentIndex != 0)
					{
						currentIndex += trisIndex;
					}

					//get the index of the current vertex in control points array
					int polyVertIndex = fbxMesh->GetPolygonVertex(polyIndex, currentIndex);
					
					FbxVector4 vertexPosition = fbxMesh->GetControlPointAt(polyVertIndex);
					vertexPosition[3] = 1.f;
					vertexPosition = globalTransform.MultT(vertexPosition);

					Vertex1P1UV4J vertex;
					vertex.position.x = static_cast<float>(vertexPosition[0]);
					vertex.position.y = static_cast<float>(vertexPosition[1]);
					vertex.position.z = static_cast<float>(vertexPosition[2]);

					FbxVector2 uvValue;

					//the UV index depends on the reference mode
					int uvIndex = 0;
					if (uvElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
					{
						uvIndex = shouldUseIndex ? uvElement->GetIndexArray().GetAt(polyVertIndex) : polyVertIndex;
					}
					else if (uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						int adjustedIndexCounter = polyIndexCounter + currentIndex;
						if (adjustedIndexCounter < indexCount)
						{
							//the UV index depends on the reference mode
							uvIndex = shouldUseIndex ? uvElement->GetIndexArray().GetAt(adjustedIndexCounter) : adjustedIndexCounter;
						}
					}

					uvValue = uvElement->GetDirectArray().GetAt(uvIndex);

					vertex.uv[0] = (float) uvValue[0];
					vertex.uv[1] = (float) uvValue[1];

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
						controlPointToVertices[polyVertIndex].push_back((int) currentMesh.m_vertices.size() - 1);
					}

					currentMesh.m_indices.push_back(index);
				}
			}

			polyIndexCounter += polySize;
		}
		//}
	}

	void FBXMeshImporter::ProcessMaterialTexture(Mesh& currentMesh, fbxsdk::FbxSurfaceMaterial* material)
	{
		unsigned int textureIndex = 0;
		fbxsdk::FbxProperty property;

		FBXSDK_FOR_EACH_TEXTURE(textureIndex)
		{
			property = material->FindProperty(fbxsdk::FbxLayerElement::sTextureChannelNames[textureIndex]);
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
						if (texture)
						{
							std::string textureType = property.GetNameAsCStr();
							fbxsdk::FbxFileTexture* fileTexture = FbxCast<fbxsdk::FbxFileTexture>(texture);

							if (fileTexture)
							{
								printf("textureType: %s\n", textureType.c_str());
								if (textureType == "DiffuseColor")
								{
									currentMesh.diffuseFileName = fileTexture->GetFileName();
								}
								else if (textureType == "SpecularColor")
								{
									currentMesh.specularFileName = fileTexture->GetFileName();
								}
								else if (textureType == "NormalMap")
								{
									currentMesh.normalFileName = fileTexture->GetFileName();
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
		FbxMesh* fbxMesh = node->GetMesh();
		unsigned int deformerCount = fbxMesh->GetDeformerCount();

		struct JointIndexWeightPair
		{
			unsigned index;
			float weight;
		};
		std::vector<std::vector<JointIndexWeightPair>> jointIndexWeightPairs;
		jointIndexWeightPairs.resize(currentMesh.m_vertices.size());

		for (unsigned deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
		{
			FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(fbxMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
			if (!currSkin)
			{
				continue;
			}

			unsigned clusterCount = currSkin->GetClusterCount();
			for (unsigned clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
			{
				FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
				std::string currJointName = currCluster->GetLink()->GetName();

				unsigned int currJointIndex = -1;
				for (unsigned i = 0; i < skeleton.joints.size(); ++i)
				{
					if (skeleton.joints[i].name == currJointName)
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
					const auto& verticesForControlPoint = controlPointToVertices[currCluster->GetControlPointIndices()[controlPointIndex]];
					for (unsigned vertexIndex = 0; vertexIndex < verticesForControlPoint.size(); ++vertexIndex)
					{
						JointIndexWeightPair jointIndexWeightPair;
						jointIndexWeightPair.index = currJointIndex;
						jointIndexWeightPair.weight = (float) currCluster->GetControlPointWeights()[controlPointIndex];
						jointIndexWeightPairs[verticesForControlPoint[vertexIndex]].push_back(jointIndexWeightPair);
					}
				}
			}
		}

		for (size_t vertexIndex = 0; vertexIndex < currentMesh.m_vertices.size(); ++vertexIndex)
		{
			std::sort(jointIndexWeightPairs[vertexIndex].begin(), jointIndexWeightPairs[vertexIndex].end(), 
				[](const JointIndexWeightPair& lhs, const JointIndexWeightPair& rhs) -> bool {
					return lhs.weight > rhs.weight;
				});

			const unsigned maxWeights = 4;
			unsigned numWeights = (unsigned) jointIndexWeightPairs[vertexIndex].size();

			if (numWeights > maxWeights)
			{
				printf("WARNING: %u Weights, %u Max\n", numWeights, maxWeights);
				numWeights = maxWeights;
			}

			float sum = 0.f;
			for (size_t j = 0; j < numWeights; ++j)
			{
				sum += jointIndexWeightPairs[vertexIndex][j].weight;
			}
			const float inverseSum = 1.f / (sum == 0.f ? 1.f : sum);
			for (size_t j = 0; j < numWeights; ++j)
			{
				jointIndexWeightPairs[vertexIndex][j].weight *= inverseSum;
			}

			size_t currentWeight;
			for (currentWeight = 0; currentWeight < numWeights; ++currentWeight)
			{
				currentMesh.m_vertices[vertexIndex].jointIndices[currentWeight] = jointIndexWeightPairs[vertexIndex][currentWeight].index;
				if (currentWeight != maxWeights - 1)
				{
					currentMesh.m_vertices[vertexIndex].jointWeights[currentWeight] = jointIndexWeightPairs[vertexIndex][currentWeight].weight;
				}
			}

			for (; currentWeight < maxWeights; ++currentWeight)
			{
				currentMesh.m_vertices[vertexIndex].jointIndices[currentWeight] = 0;
				if (currentWeight != maxWeights - 1)
				{
					currentMesh.m_vertices[vertexIndex].jointWeights[currentWeight] = 0.f;
				}
			}
		}
	}
}
