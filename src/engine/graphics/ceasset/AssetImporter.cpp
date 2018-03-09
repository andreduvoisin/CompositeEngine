#include "AssetImporter.h"

#include "AssetTraits.h"
#include "InputFileStream.h"

#include "graphics\skeleton\Skeleton.h"
#include "graphics\mesh\Mesh.h"
#include "graphics\animation\Animation.h"
#include "graphics\texture\Texture.h"

namespace CE
{
	bool AssetImporter::ImportSkeletonMeshesAnimationsTexture(
		const char* fileName, 
		Skeleton& outSkeleton, 
		Meshes& outMeshes,
		Animations& outAnimations, 
		Texture& outTexture)
	{
		InputFileStream stream(fileName);

		if (!stream.IsValid())
		{
			return false;
		}

		char header[8];
		stream.Read(header, 8);
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

		while (stream.HasData())
		{
			const auto assetType = stream.Read<AssetType>();

			switch (assetType)
			{
				case AssetType::SKELETON:
				{
					const auto jointCount = stream.Read<unsigned>();
					outSkeleton.joints.resize(jointCount);
					for (unsigned jointIndex = 0; jointIndex < jointCount; ++jointIndex)
					{
						Joint& joint = outSkeleton.joints[jointIndex];
						stream.Read(joint.inverseBindPose);
						stream.Read(joint.name);
						stream.Read(joint.parentIndex);
					}

					break;
				}

				case AssetType::MESH:
				{
					outMeshes.push_back(Mesh());
					Mesh& mesh = outMeshes.back();

					const auto verticesCount = stream.Read<unsigned>();
					mesh.m_vertices.resize(verticesCount);
					stream.Read(mesh.m_vertices.data(), verticesCount);

					const auto indicesCount = stream.Read<unsigned>();
					mesh.m_indices.resize(indicesCount);
					stream.Read(mesh.m_indices.data(), indicesCount);

					stream.Read(mesh.m_diffuseMapName);
					stream.Read(mesh.m_specularMapName);
					stream.Read(mesh.m_normalMapName);

					break;
				}

				case AssetType::ANIMATION:
				{
					outAnimations.push_back(Animation());
					Animation& animation = outAnimations.back();
					animation.rotations.resize(outSkeleton.joints.size());
					animation.scales.resize(outSkeleton.joints.size());

					stream.Read(animation.name);

					const auto translationsCount = stream.Read<unsigned>();
					animation.translations.resize(translationsCount);
					for (unsigned translationsIndex = 0; translationsIndex < translationsCount; ++translationsIndex)
					{
						auto& translations = animation.translations[translationsIndex];
						const auto keyCount = stream.Read<unsigned>();
						translations.resize(keyCount);
						stream.Read(translations.data(), keyCount);
					}

					const auto rotationsCount = stream.Read<unsigned>();
					animation.rotations.resize(rotationsCount);
					for (unsigned rotationsIndex = 0; rotationsIndex < rotationsCount; ++rotationsIndex)
					{
						auto& rotations = animation.rotations[rotationsIndex];
						const auto keyCount = stream.Read<unsigned>();
						rotations.resize(keyCount);
						stream.Read(rotations.data(), keyCount);
					}

					const auto scalesCount = stream.Read<unsigned>();
					animation.scales.resize(scalesCount);
					for (unsigned scalesIndex = 0; scalesIndex < scalesCount; ++scalesIndex)
					{
						auto& scales = animation.scales[scalesIndex];
						const auto keyCount = stream.Read<unsigned>();
						scales.resize(keyCount);
						stream.Read(scales.data(), keyCount);
					}

					stream.Read(animation.duration);

					break;
				}

				case AssetType::TEXTURE:
				{
					stream.Read(outTexture.width);
					stream.Read(outTexture.height);
					stream.Read(outTexture.channels);

					// todo: delete. also probs shouldn't be here anyways
					outTexture.data = new unsigned char[outTexture.width * outTexture.height * outTexture.channels];
					stream.Read(outTexture.data, outTexture.width * outTexture.height * outTexture.channels);

					break;
				}
			}
		}

		return stream.IsValid();
	}
}
