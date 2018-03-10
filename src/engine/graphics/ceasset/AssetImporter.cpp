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
					for (auto& joint : outSkeleton.joints)
					{
						stream >> joint.inverseBindPose;
						stream >> joint.name;
						stream >> joint.parentIndex;
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

					stream >> mesh.m_diffuseMapName;
					stream >> mesh.m_specularMapName;
					stream >> mesh.m_normalMapName;

					break;
				}

				case AssetType::ANIMATION:
				{
					outAnimations.push_back(Animation());
					Animation& animation = outAnimations.back();

					stream >> animation.name;

					const auto translationsCount = stream.Read<unsigned>();
					animation.translations.resize(translationsCount);
					for (auto& translations : animation.translations)
					{
						const auto keyCount = stream.Read<unsigned>();
						translations.resize(keyCount);
						stream.Read(translations.data(), keyCount);
					}

					const auto rotationsCount = stream.Read<unsigned>();
					animation.rotations.resize(rotationsCount);
					for (auto& rotations : animation.rotations)
					{
						const auto keyCount = stream.Read<unsigned>();
						rotations.resize(keyCount);
						stream.Read(rotations.data(), keyCount);
					}

					const auto scalesCount = stream.Read<unsigned>();
					animation.scales.resize(scalesCount);
					for (auto& scales : animation.scales)
					{
						const auto keyCount = stream.Read<unsigned>();
						scales.resize(keyCount);
						stream.Read(scales.data(), keyCount);
					}

					stream >> animation.duration;

					break;
				}

				case AssetType::TEXTURE:
				{
					stream >> outTexture.width;
					stream >> outTexture.height;
					stream >> outTexture.channels;

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
