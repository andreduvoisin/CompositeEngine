#include "AssetSerializer.h"

#include "AssetTraits.h"
#include "OutputFileStream.h"

#include "graphics\skeleton\Skeleton.h"
#include "graphics\mesh\Mesh.h"
#include "graphics\animation\Animation.h"
#include "graphics\texture\Texture.h"

namespace CE
{
	AssetSerializer::AssetSerializer(OutputFileStream& stream)
		: stream(stream)
	{

	}

	void AssetSerializer::WriteHeader()
	{
		stream.Write(ASSET_FILE_HEADER, ASSET_FILE_HEADER_LENGTH);
	}

	void AssetSerializer::WriteSkeleton(const Skeleton& skeleton)
	{
		stream << AssetType::SKELETON;

		stream << static_cast<unsigned>(skeleton.joints.size());

		for (const Joint& joint : skeleton.joints)
		{
			stream << joint.inverseBindPose;
			stream.Write(joint.name.data(), joint.name.size() + 1);
			stream << joint.parentIndex;
		}
	}

	void AssetSerializer::WriteMesh(const Mesh& mesh)
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

	void AssetSerializer::WriteMeshes(const Meshes& meshes)
	{
		for (const Mesh& mesh : meshes)
		{
			WriteMesh(mesh);
		}
	}

	void AssetSerializer::WriteAnimation(const Animation& animation)
	{
		stream << AssetType::ANIMATION;

		stream.Write(animation.name.data(), animation.name.size() + 1);

		WriteAnimationSQT(animation.translations);
		WriteAnimationSQT(animation.rotations);
		WriteAnimationSQT(animation.scales);

		stream << animation.duration;
	}

	void AssetSerializer::WriteAnimations(const Animations& animations)
	{
		for (const Animation& animation : animations)
		{
			WriteAnimation(animation);
		}
	}

	void AssetSerializer::WriteTexture(const Texture& texture)
	{
		stream << AssetType::TEXTURE;

		stream << texture.width;
		stream << texture.height;
		stream << texture.channels;
		stream.Write(texture.data, texture.width * texture.height * texture.channels);
	}

	template<typename T>
	void AssetSerializer::WriteAnimationSQT(const std::vector<std::vector<T>>& components)
	{
		stream << static_cast<unsigned>(components.size());
		for (const auto& component : components)
		{
			stream << static_cast<unsigned>(component.size());
			stream.Write(component.data(), component.size());
		}
	}
}
