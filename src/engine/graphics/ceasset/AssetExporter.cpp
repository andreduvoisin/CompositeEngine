#include "AssetExporter.h"

#include "AssetTraits.h"
#include "OutputFileStream.h"

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
		OutputFileStream stream(fileName);

		if (!stream.IsValid())
		{
			return false;
		}


		stream << 'C' << 'E' << 'A' << 'S' << 'S' << 'E' << 'T' << '\0';


		stream << AssetType::SKELETON;

		stream << static_cast<unsigned>(skeleton.joints.size());

		for (const Joint& joint : skeleton.joints)
		{
			stream << joint.inverseBindPose;
			stream.Write(joint.name.data(), joint.name.size() + 1);
			stream << joint.parentIndex;
		}


		for (const Mesh& mesh : meshes)
		{
			stream << AssetType::MESH;

			stream << static_cast<unsigned>(mesh.m_vertices.size());
			stream.Write(mesh.m_vertices.data(), mesh.m_vertices.size());

			stream << static_cast<unsigned>(mesh.m_indices.size());
			stream.Write(mesh.m_indices.data(), mesh.m_indices.size());

			stream.Write(mesh.m_diffuseMapName.data(), mesh.m_diffuseMapName.size() + 1);
			stream.Write(mesh.m_specularMapName.data(), mesh.m_specularMapName.size() + 1);
			stream.Write(mesh.m_normalMapName.data(), mesh.m_normalMapName.size() + 1);
		}


		for (const Animation& animation : animations)
		{
			stream << AssetType::ANIMATION;

			stream.Write(animation.name.data(), animation.name.size() + 1);

			stream << static_cast<unsigned>(animation.translations.size());
			for (const auto& translations : animation.translations)
			{
				stream << static_cast<unsigned>(translations.size());
				stream.Write(translations.data(), translations.size());
			}

			stream << static_cast<unsigned>(animation.rotations.size());
			for (const auto& rotations : animation.rotations)
			{
				stream << static_cast<unsigned>(rotations.size());
				stream.Write(rotations.data(), rotations.size());
			}

			stream << static_cast<unsigned>(animation.scales.size());
			for (const auto& scales : animation.scales)
			{
				stream << static_cast<unsigned>(scales.size());
				stream.Write(scales.data(), scales.size());
			}

			stream << animation.duration;
		}


		stream << AssetType::TEXTURE;
		stream << texture.width;
		stream << texture.height;
		stream << texture.channels;
		stream.Write(texture.data, texture.width * texture.height * texture.channels);


		return stream.IsValid();
	}
}
