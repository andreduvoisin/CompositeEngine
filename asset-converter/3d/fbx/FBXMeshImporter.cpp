#include "FBXMeshImporter.h"

#include "FBXUtilities.h"
#include "FBXValidator.h"
#include "graphics/mesh/Mesh.h"
#include "graphics/skeleton/Skeleton.h"

#include <algorithm>
#include <fbxsdk.h>
#include <iostream>

namespace CE
{
    FBXMeshImporter::FBXMeshImporter(
            fbxsdk::FbxManager* fbxManager,
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

        fbxsdk::FbxImporter* importer = fbxsdk::FbxImporter::Create(fbxManager, "");
        fbxsdk::FbxScene* scene = fbxsdk::FbxScene::Create(fbxManager, "");

        if (!importer->Initialize(fileName, -1, fbxManager->GetIOSettings()))
        {
            return false;
        }

        if (!importer->Import(scene))
        {
            return false;
        }

        importer->Destroy();

        fbxsdk::FbxNode* rootNode = scene->GetRootNode();
        if (rootNode == nullptr)
        {
            return false;
        }

        ParseNodes(rootNode, scene);

        return true;
    }

    void FBXMeshImporter::ParseNodes(fbxsdk::FbxNode* rootNode, fbxsdk::FbxScene* scene)
    {
        PrintNode(rootNode);

        const int meshCount = scene->GetSrcObjectCount<fbxsdk::FbxMesh>();
        outMeshes->resize(meshCount);

        for (int i = 0; i < meshCount; i++)
        {
            auto* pMesh = scene->GetSrcObject<fbxsdk::FbxMesh>(i);
            fbxsdk::FbxNode* pNode = pMesh->GetNode();

            Mesh& currentMesh = outMeshes->at(i);
            controlPointToVertices.clear();

            unsigned int materialCount = pNode->GetMaterialCount();
            for (unsigned int i = 0; i < materialCount; ++i)
            {
                fbxsdk::FbxSurfaceMaterial* surfaceMaterial = pNode->GetMaterial(i);
                ProcessMaterialTexture(currentMesh, surfaceMaterial);
            }

            ProcessVertices(currentMesh, pMesh);
            ProcessSkinnedMesh(currentMesh, pNode, scene);

            std::cout << "currentMesh.m_diffuseMapName: " << currentMesh.m_diffuseMapName.c_str() << "\n";
            std::cout << "currentMesh.m_specularMapName: " << currentMesh.m_specularMapName.c_str() << "\n";
            std::cout << "currentMesh.m_normalMapName: " << currentMesh.m_normalMapName.c_str() << "\n";
        }
    }

    void FBXMeshImporter::ProcessVertices(Mesh& currentMesh, fbxsdk::FbxMesh* mesh)
    {
        // get all UV set names
        fbxsdk::FbxStringList lUVSetNameList;
        mesh->GetUVSetNames(lUVSetNameList);

        for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
        {
            const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
            // const fbxsdk::FbxGeometryElementUV* lUVElement = mesh->GetElementUV(lUVSetName);
            std::cout << "lUVSetName: " << lUVSetName << "\n";
        }

        currentMesh.m_indices.reserve(mesh->GetControlPointsCount());

        int quadCount = 0;
        int triCount = 0;
        int unknownCount = 0;

        // iterating over all uv sets
        // for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
        //{
        // get lUVSetIndex-th uv set
        // const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
        // const fbxsdk::FbxGeometryElementUV* lUVElement = mesh->GetElementUV(lUVSetName);
        const fbxsdk::FbxGeometryElementUV* lUVElement =
                mesh->GetElementUV(0); // , fbxsdk::FbxLayerElement::eTextureDiffuse);

        if (lUVElement == nullptr)
        {
            // continue;
            return;
        }

        // only support mapping mode eByPolygonVertex and eByControlPoint
        if (lUVElement->GetMappingMode() != fbxsdk::FbxGeometryElement::eByPolygonVertex
            && lUVElement->GetMappingMode() != fbxsdk::FbxGeometryElement::eByControlPoint)
        {
            // continue;
            return;
        }

        // index array, where holds the index referenced to the uv data
        const bool lUseIndex = lUVElement->GetReferenceMode() != fbxsdk::FbxGeometryElement::eDirect;
        const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

        // iterating through the data by polygon
        const int lPolyCount = mesh->GetPolygonCount();
        // printf("lUVSetName: %s\n", lUVSetName);
        std::cout << "lPolyCount: " << lPolyCount << "\n";

        const fbxsdk::FbxAMatrix globalTransform = mesh->GetNode()->EvaluateGlobalTransform();

        int lPolyIndexCounter = 0;
        for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
        {
            // build the max index array that we need to pass into MakePoly
            const int lPolySize = mesh->GetPolygonSize(lPolyIndex);
            if (lPolySize < 3)
            {
                std::cout << "degenerate polygon!\n";
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

                    // get the index of the current vertex in control points array
                    int lPolyVertIndex = mesh->GetPolygonVertex(lPolyIndex, currentIndex);

                    fbxsdk::FbxVector4 vertexPosition = mesh->GetControlPointAt(lPolyVertIndex);
                    vertexPosition[3] = 1.F;
                    vertexPosition = globalTransform.MultT(vertexPosition);

                    Vertex1P1UV4J vertex;
                    vertex.position.x = static_cast<float>(vertexPosition[0]);
                    vertex.position.y = static_cast<float>(vertexPosition[1]);
                    vertex.position.z = static_cast<float>(vertexPosition[2]);

                    fbxsdk::FbxVector2 lUVValue;

                    // the UV index depends on the reference mode
                    int lUVIndex = 0;
                    if (lUVElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
                    {
                        lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;
                    }
                    else if (lUVElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex)
                    {
                        int adjustedIndexCounter = lPolyIndexCounter + currentIndex;
                        if (adjustedIndexCounter < lIndexCount)
                        {
                            // the UV index depends on the reference mode
                            lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(adjustedIndexCounter)
                                                 : adjustedIndexCounter;
                        }
                    }

                    lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

                    vertex.uv[0] = (float) lUVValue[0];
                    vertex.uv[1] = (float) lUVValue[1];

                    size_t index;
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
                        controlPointToVertices[lPolyVertIndex].push_back((int) currentMesh.m_vertices.size() - 1);
                    }

                    currentMesh.m_indices.push_back(index);
                }
            }

            lPolyIndexCounter += lPolySize;
        }
        //}
    }

    void FBXMeshImporter::ProcessMaterialTexture(Mesh& currentMesh, fbxsdk::FbxSurfaceMaterial* material)
    {
        for (unsigned int textureIndex = 0; textureIndex < fbxsdk::FbxLayerElement::sTypeTextureCount; ++textureIndex)
        {
            fbxsdk::FbxProperty property =
                    material->FindProperty(fbxsdk::FbxLayerElement::sTextureChannelNames[textureIndex]);
            if (property.IsValid())
            {
                unsigned int textureCount = property.GetSrcObjectCount<fbxsdk::FbxTexture>();
                std::cout << "textureCount: " << textureCount << "\n";
                for (unsigned int i = 0; i < textureCount; ++i)
                {
                    auto* layeredTexture = property.GetSrcObject<fbxsdk::FbxLayeredTexture>(i);
                    if (layeredTexture != nullptr)
                    {
                        std::cout << "Layered Texture is currently unsupported\n";
                        continue;
                    }

                    auto* texture = property.GetSrcObject<fbxsdk::FbxTexture>(i);
                    if (texture != nullptr)
                    {
                        std::string textureType = property.GetNameAsCStr();
                        auto* fileTexture = fbxsdk::FbxCast<fbxsdk::FbxFileTexture>(texture);

                        if (fileTexture != nullptr)
                        {
                            std::cout << "textureType: " << textureType.c_str() << "\n";
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

    void FBXMeshImporter::ProcessSkinnedMesh(Mesh& currentMesh, fbxsdk::FbxNode* node, fbxsdk::FbxScene* /*scene*/)
    {
        fbxsdk::FbxMesh* currMesh = node->GetMesh();
        int numOfDeformers = currMesh->GetDeformerCount();

        struct JointIndexWeightPair
        {
            unsigned index;
            float weight;
        };
        std::vector<std::vector<JointIndexWeightPair>> jointIndexWeightPairs;
        jointIndexWeightPairs.resize(currentMesh.m_vertices.size());

        for (int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
        {
            auto* currSkin = reinterpret_cast<fbxsdk::FbxSkin*>(
                    currMesh->GetDeformer(deformerIndex, fbxsdk::FbxDeformer::eSkin));
            if (currSkin == nullptr)
            {
                continue;
            }

            int numOfClusters = currSkin->GetClusterCount();
            for (int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
            {
                fbxsdk::FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
                std::string currJointName = currCluster->GetLink()->GetName();

                unsigned int currJointIndex = -1;
                for (size_t i = 0; i < skeleton.joints.size(); ++i)
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
                    const auto& verticesForControlPoint =
                            controlPointToVertices[currCluster->GetControlPointIndices()[controlPointIndex]];
                    for (int vertexIndex : verticesForControlPoint)
                    {
                        JointIndexWeightPair jointIndexWeightPair {};
                        jointIndexWeightPair.index = currJointIndex;
                        jointIndexWeightPair.weight = (float) currCluster->GetControlPointWeights()[controlPointIndex];
                        jointIndexWeightPairs[vertexIndex].push_back(jointIndexWeightPair);
                    }
                }
            }
        }

        for (size_t i = 0; i < currentMesh.m_vertices.size(); ++i)
        {
            std::sort(
                    jointIndexWeightPairs[i].begin(),
                    jointIndexWeightPairs[i].end(),
                    [](const JointIndexWeightPair& lhs, const JointIndexWeightPair& rhs) -> bool {
                        return lhs.weight > rhs.weight;
                    });

            const unsigned maxWeights = 4;
            auto numWeights = (unsigned) jointIndexWeightPairs[i].size();

            if (numWeights > maxWeights)
            {
                std::cout << "WARNING: " << numWeights << " Weights, " << maxWeights << " Max\n";
                numWeights = maxWeights;
            }

            float sum = 0.F;
            for (size_t j = 0; j < numWeights; ++j)
            {
                sum += jointIndexWeightPairs[i][j].weight;
            }
            const float inverseSum = 1.F / (sum == 0.F ? 1.F : sum);
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
                    currentMesh.m_vertices[i].jointWeights[currentWeight] =
                            jointIndexWeightPairs[i][currentWeight].weight;
                }
            }

            for (; currentWeight < maxWeights; ++currentWeight)
            {
                currentMesh.m_vertices[i].jointIndices[currentWeight] = 0;
                if (currentWeight != maxWeights - 1)
                {
                    currentMesh.m_vertices[i].jointWeights[currentWeight] = 0.F;
                }
            }
        }
    }
}
