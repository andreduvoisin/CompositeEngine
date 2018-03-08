#include "AssetExporter.h"

#include "AssetTraits.h"

#include "graphics\skeleton\Skeleton.h"
#include "graphics\mesh\Mesh.h"
#include "graphics\animation\Animation.h"
#include "graphics\texture\Texture.h"

#include <fstream>

namespace CE
{
	bool AssetExporter::ExportSkeletonMeshesAnimationsTexture(
		const char* fileName,
		const Skeleton& skeleton,
		const Meshes& meshes,
		const Animations& animations,
		const Texture& texture)
	{
		std::ofstream stream;
		stream.open(fileName, std::ios::out | std::ios::binary);

		if (!stream.is_open())
		{
			return false;
		}


		stream << 'C' << 'E' << 'A' << 'S' << 'S' << 'E' << 'T' << '\0';

		AssetType skeletonType = AssetType::SKELETON;
		stream.write(reinterpret_cast<const char*>(&skeletonType), sizeof(skeletonType));

		const unsigned jointCount = skeleton.joints.size();
		stream.write(reinterpret_cast<const char*>(&jointCount), sizeof(jointCount));

		for (const Joint& joint : skeleton.joints)
		{
			stream.write(reinterpret_cast<const char*>(&joint.inverseBindPose), sizeof(glm::mat4));
			stream.write(joint.name.data(), joint.name.size() + 1);
			stream.write(reinterpret_cast<const char*>(&joint.parentIndex), sizeof(joint.parentIndex));
		}


		for (const Mesh& mesh : meshes)
		{
			AssetType meshType = AssetType::MESH;
			stream.write(reinterpret_cast<const char*>(&meshType), sizeof(meshType));

			const unsigned verticesCount = mesh.m_vertices.size();
			stream.write(reinterpret_cast<const char*>(&verticesCount), sizeof(verticesCount));
			stream.write(reinterpret_cast<const char*>(mesh.m_vertices.data()), sizeof(Vertex1P1UV4J) * verticesCount);


			const unsigned indicesCount = mesh.m_indices.size();
			stream.write(reinterpret_cast<const char*>(&indicesCount), sizeof(indicesCount));
			stream.write(reinterpret_cast<const char*>(mesh.m_indices.data()), sizeof(unsigned int) * indicesCount);

			stream.write(mesh.m_diffuseMapName.data(), mesh.m_diffuseMapName.size() + 1);
			stream.write(mesh.m_specularMapName.data(), mesh.m_specularMapName.size() + 1);
			stream.write(mesh.m_normalMapName.data(), mesh.m_normalMapName.size() + 1);
		}


		for (const Animation& animation : animations)
		{
			AssetType animationType = AssetType::ANIMATION;
			stream.write(reinterpret_cast<const char*>(&animationType), sizeof(animationType));

			stream.write(animation.name.data(), animation.name.size() + 1);

			for (const auto& translations : animation.translations)
			{
				const unsigned translationsCount = translations.size();
				stream.write(reinterpret_cast<const char*>(&translationsCount), sizeof(translationsCount));
				stream.write(reinterpret_cast<const char*>(translations.data()), sizeof(TranslationKey) * translationsCount);
			}

			for (const auto& rotations : animation.rotations)
			{
				const unsigned rotationsCount = rotations.size();
				stream.write(reinterpret_cast<const char*>(&rotationsCount), sizeof(rotationsCount));
				stream.write(reinterpret_cast<const char*>(rotations.data()), sizeof(RotationKey) * rotationsCount);
			}

			for (const auto& scales : animation.scales)
			{
				const unsigned scalesCount = scales.size();
				stream.write(reinterpret_cast<const char*>(&scalesCount), sizeof(scalesCount));
				stream.write(reinterpret_cast<const char*>(scales.data()), sizeof(ScaleKey) * scalesCount);
			}

			stream.write(reinterpret_cast<const char*>(&animation.duration), sizeof(animation.duration));
		}


		AssetType textureType = AssetType::TEXTURE;
		stream.write(reinterpret_cast<const char*>(&textureType), sizeof(textureType));
		stream.write(reinterpret_cast<const char*>(&texture.width), sizeof(texture.width));
		stream.write(reinterpret_cast<const char*>(&texture.height), sizeof(texture.height));
		stream.write(reinterpret_cast<const char*>(&texture.channels), sizeof(texture.channels));
		stream.write(reinterpret_cast<const char*>(texture.data), texture.width * texture.height * texture.channels);


		stream.close();
		return true;
	}
}
