#include "AssetImporter.h"

#include "AssetTraits.h"

#include "graphics\skeleton\Skeleton.h"
#include "graphics\mesh\Mesh.h"
#include "graphics\animation\Animation.h"
#include "graphics\texture\Texture.h"

#include <fstream>
#include <string>

namespace CE
{
	bool AssetImporter::ImportSkeletonMeshesAnimationsTexture(
		const char* fileName, 
		Skeleton& outSkeleton, 
		Meshes& outMeshes,
		Animations& outAnimations, 
		Texture& outTexture)
	{
		std::ifstream stream;
		stream.open(fileName, std::ios::in | std::ios::binary);

		if (!stream.is_open())
		{
			return false;
		}

		char header[8];
		stream.read(header, 8);
		if (!(header[0] == 'C'
			&& header[1] == 'E'
			&& header[2] == 'A'
			&& header[3] == 'S'
			&& header[4] == 'S'
			&& header[5] == 'E'
			&& header[6] == 'T'
			&& header[7] == '\0'))
		{
			return false;
		}

		AssetType assetType;
		stream.read(reinterpret_cast<char*>(&assetType), sizeof(assetType));

		while (!stream.eof())
		{
			switch (assetType)
			{
				case AssetType::SKELETON:
				{
					unsigned jointCount;
					stream.read(reinterpret_cast<char*>(&jointCount), sizeof(jointCount));

					outSkeleton.joints.resize(jointCount);

					for (unsigned jointIndex = 0; jointIndex < jointCount; ++jointIndex)
					{
						Joint& joint = outSkeleton.joints[jointIndex];
						stream.read(reinterpret_cast<char*>(&joint.inverseBindPose), sizeof(joint.inverseBindPose));
						std::getline(stream, joint.name, '\0');
						stream.read(reinterpret_cast<char*>(&joint.parentIndex), sizeof(joint.parentIndex));
					}

					break;
				}

				case AssetType::MESH:
				{
					outMeshes.push_back(Mesh());
					Mesh& mesh = outMeshes.back();

					unsigned verticesCount;
					stream.read(reinterpret_cast<char*>(&verticesCount), sizeof(verticesCount));
					mesh.m_vertices.resize(verticesCount);
					stream.read(reinterpret_cast<char*>(mesh.m_vertices.data()), sizeof(Vertex1P1UV4J) * verticesCount);

					unsigned indicesCount;
					stream.read(reinterpret_cast<char*>(&indicesCount), sizeof(indicesCount));
					mesh.m_indices.resize(indicesCount);
					stream.read(reinterpret_cast<char*>(mesh.m_indices.data()), sizeof(unsigned int) * indicesCount);

					std::getline(stream, mesh.m_diffuseMapName, '\0');
					std::getline(stream, mesh.m_specularMapName, '\0');
					std::getline(stream, mesh.m_normalMapName, '\0');

					break;
				}

				case AssetType::ANIMATION:
				{
					outAnimations.push_back(Animation());
					Animation& animation = outAnimations.back();
					animation.translations.resize(outSkeleton.joints.size());
					animation.rotations.resize(outSkeleton.joints.size());
					animation.scales.resize(outSkeleton.joints.size());

					std::getline(stream, animation.name, '\0');

					for (unsigned translationsIndex = 0; translationsIndex < outSkeleton.joints.size(); ++translationsIndex)
					{
						unsigned translationsCount;
						stream.read(reinterpret_cast<char*>(&translationsCount), sizeof(translationsCount));
						animation.translations[translationsIndex].resize(translationsCount);
						stream.read(reinterpret_cast<char*>(animation.translations[translationsIndex].data()), sizeof(TranslationKey) * translationsCount);
					}

					for (unsigned rotationsIndex = 0; rotationsIndex < outSkeleton.joints.size(); ++rotationsIndex)
					{
						unsigned rotationsCount;
						stream.read(reinterpret_cast<char*>(&rotationsCount), sizeof(rotationsCount));
						animation.rotations[rotationsIndex].resize(rotationsCount);
						stream.read(reinterpret_cast<char*>(animation.rotations[rotationsIndex].data()), sizeof(RotationKey) * rotationsCount);
					}

					for (unsigned scalesIndex = 0; scalesIndex < outSkeleton.joints.size(); ++scalesIndex)
					{
						unsigned scalesCount;
						stream.read(reinterpret_cast<char*>(&scalesCount), sizeof(scalesCount));
						animation.scales[scalesIndex].resize(scalesCount);
						stream.read(reinterpret_cast<char*>(animation.scales[scalesIndex].data()), sizeof(ScaleKey) * scalesCount);
					}

					stream.read(reinterpret_cast<char*>(&animation.duration), sizeof(animation.duration));

					break;
				}

				case AssetType::TEXTURE:
				{
					stream.read(reinterpret_cast<char*>(&outTexture.width), sizeof(outTexture.width));
					stream.read(reinterpret_cast<char*>(&outTexture.height), sizeof(outTexture.height));
					stream.read(reinterpret_cast<char*>(&outTexture.channels), sizeof(outTexture.channels));
					// todo: delete. also probs shouldn't be here anyways
					outTexture.data = new unsigned char[outTexture.width * outTexture.height * outTexture.channels];
					stream.read(reinterpret_cast<char*>(outTexture.data), outTexture.width * outTexture.height * outTexture.channels);

					break;
				}
			}

			stream.read(reinterpret_cast<char*>(&assetType), sizeof(assetType));
		}

		stream.close();
		return true;
	}
}
