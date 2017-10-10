#include "MeshData.h"

#include <fbxsdk.h>

#include <assert.h>

/* Tab character ("\t") counter */
int numTabs = 0;

/**
* Print the required number of tabs.
*/
void PrintTabs() {
	for (int i = 0; i < numTabs; i++)
		printf("\t");
}

/**
* Return a string-based representation based on the attribute type.
*/
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
	switch (type) {
		case FbxNodeAttribute::eUnknown: return "unidentified";
		case FbxNodeAttribute::eNull: return "null";
		case FbxNodeAttribute::eMarker: return "marker";
		case FbxNodeAttribute::eSkeleton: return "skeleton";
		case FbxNodeAttribute::eMesh: return "mesh";
		case FbxNodeAttribute::eNurbs: return "nurbs";
		case FbxNodeAttribute::ePatch: return "patch";
		case FbxNodeAttribute::eCamera: return "camera";
		case FbxNodeAttribute::eCameraStereo: return "stereo";
		case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
		case FbxNodeAttribute::eLight: return "light";
		case FbxNodeAttribute::eOpticalReference: return "optical reference";
		case FbxNodeAttribute::eOpticalMarker: return "marker";
		case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
		case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
		case FbxNodeAttribute::eBoundary: return "boundary";
		case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
		case FbxNodeAttribute::eShape: return "shape";
		case FbxNodeAttribute::eLODGroup: return "lodgroup";
		case FbxNodeAttribute::eSubDiv: return "subdiv";
		default: return "unknown";
	}
}

/**
* Print an attribute.
*/
void PrintAttribute(FbxNodeAttribute* pAttribute) {
	if (!pAttribute) return;

	FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
	FbxString attrName = pAttribute->GetName();
	PrintTabs();
	// Note: to retrieve the character array of a FbxString, use its Buffer() method.
	printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

/**
* Print a node, its attributes, and all its children recursively.
*/
void PrintNode(FbxNode* pNode) {
	PrintTabs();
	const char* nodeName = pNode->GetName();
	FbxDouble3 translation = pNode->LclTranslation.Get();
	FbxDouble3 rotation = pNode->LclRotation.Get();
	FbxDouble3 scaling = pNode->LclScaling.Get();

	// Print the contents of the node.
	printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
		nodeName,
		translation[0], translation[1], translation[2],
		rotation[0], rotation[1], rotation[2],
		scaling[0], scaling[1], scaling[2]
	);
	numTabs++;

	// Print the node's attributes.
	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
		PrintAttribute(pNode->GetNodeAttributeByIndex(i));

	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
		PrintNode(pNode->GetChild(j));

	numTabs--;
	PrintTabs();
	printf("</node>\n");
}

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

		ParseNodes(pFbxRootNode);

		return true;
	}

	void MeshData::ParseNodes(FbxNode* pFbxRootNode)
	{
		PrintNode(pFbxRootNode);

		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			//if (strcmp(pFbxChildNode->GetName(), "Paladin_J_Nordstrom_Sword") != 0)
			//{
			//	continue;
			//}

			ParseNodes(pFbxChildNode);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
			{
				continue;
			}

			if (pFbxChildNode->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eMesh)
			{
				continue;
			}

			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();

			unsigned int materialCount = pFbxChildNode->GetMaterialCount();
			for (unsigned int i = 0; i < materialCount; ++i)
			{
				FbxSurfaceMaterial* surfaceMaterial = pFbxChildNode->GetMaterial(i);
				ProcessMaterialTexture(surfaceMaterial);
			}

			LoadInformation(pMesh);

			printf("m_diffuseMapName: %s\n", m_diffuseMapName.c_str());
			printf("m_specularMapName: %s\n", m_specularMapName.c_str());
			printf("m_normalMapName: %s\n", m_normalMapName.c_str());
		}
	}

	void MeshData::LoadInformation(FbxMesh* pMesh)
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
		m_indices.reserve(pMesh->GetControlPointsCount());

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

				if (lPolySize == 4)
				{
					quadCount++;
				}
				else if (lPolySize == 3)
				{
					triCount++;
				}
				else
				{
					unknownCount++;
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

						Vertex vertex;
						vertex.position.x = (float)pVertices[lPolyVertIndex][0];
						vertex.position.y = (float)pVertices[lPolyVertIndex][1];
						vertex.position.z = (float)pVertices[lPolyVertIndex][2];

						//printf("pVertices[lPolyVertIndex]: %f, %f, %f, %f\n", (float)pVertices[lPolyVertIndex][0], (float)pVertices[lPolyVertIndex][1], (float)pVertices[lPolyVertIndex][2], (float)pVertices[lPolyVertIndex][3]);


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
						for (index = 0; index < m_vertices.size(); ++index)
						{
							if (vertex == m_vertices[index])
							{
								break;
							}
						}

						if (index == m_vertices.size())
						{
							m_vertices.push_back(vertex);
							//m_vertices.push_back(vertex);
						}

						m_indices.push_back(index);

						//User TODO:
						//Print out the value of UV(lUVValue) or log it to a file
						//printf("%f, %f\n", lUVValue.mData[0], lUVValue.mData[1]);
					}
				}

				lPolyIndexCounter += lPolySize;
			}
		//}

		printf("quadCount: %i\n", quadCount);
		printf("triCount: %i\n", triCount);
		printf("unknownCount: %i\n", unknownCount);
	}

	void MeshData::ProcessMaterialTexture(fbxsdk::FbxSurfaceMaterial* inMaterial)
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
						throw std::exception("Layered Texture is currently unsupported\n");
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
									m_diffuseMapName = fileTexture->GetFileName();
								}
								else if (textureType == "SpecularColor")
								{
									m_specularMapName = fileTexture->GetFileName();
								}
								else if (textureType == "NormalMap")
								{
									m_normalMapName = fileTexture->GetFileName();
								}
							}
						}
					}
				}
			}
		}
	}

	void MeshData::ProcessAnimation(FbxNode* node)
	{
		ProcessSkeletonHierarchy(node);

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

				if (currJointIndex == -1)
				{
					printf("Couldn't find joint index.\n");
					continue; // should this break? or return even?
				}

				FbxAMatrix transformMatrix = currCluster->GetTransformMatrix(transformMatrix);
				FbxAMatrix transformLinkMatrix = currCluster->GetTransformLinkMatrix(transformLinkMatrix);
				FbxAMatrix globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;
				for (unsigned i = 0; i < 16; ++i)
				{
					m_skeleton.joints[currJointIndex].inverseBindPose[i][i % 4] = globalBindposeInverseMatrix[i][i % 4];
				}

				unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
				for (unsigned i = 0; i < numOfIndices; ++i)
				{

				}
			}
		}
	}

	void MeshData::ProcessSkeletonHierarchy(FbxNode* inRootNode)
	{
		for (int childIndex = 0; childIndex < inRootNode->GetChildCount(); ++childIndex)
		{
			FbxNode* currNode = inRootNode->GetChild(childIndex);
			ProcessSkeletonHierarchyRecursively(currNode, 0, 0, -1);
		}
	}

	void MeshData::ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int inDepth, int myIndex, int inParentIndex)
	{
		if (inNode->GetNodeAttribute() && inNode->GetNodeAttribute()->GetAttributeType() && inNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			Joint currJoint;
			currJoint.parentIndex = inParentIndex;
			currJoint.name = inNode->GetName();
			m_skeleton.joints.push_back(currJoint);
		}
		for (int i = 0; i < inNode->GetChildCount(); i++)
		{
			ProcessSkeletonHierarchyRecursively(inNode->GetChild(i), inDepth + 1, m_skeleton.joints.size(), myIndex);
		}
	}

	void MeshData::Draw()
	{

	}
}
