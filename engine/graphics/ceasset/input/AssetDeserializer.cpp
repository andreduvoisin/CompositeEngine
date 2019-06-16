#include "AssetDeserializer.h"

#include "graphics/ceasset/AssetTraits.h"
#include "InputFileStream.h"

#include "graphics/skeleton/Skeleton.h"
#include "graphics/mesh/Mesh.h"
#include "graphics/animation/Animation.h"
#include "graphics/texture/Texture.h"

namespace CE
{
	AssetDeserializer::AssetDeserializer(InputFileStream& stream)
		: stream(stream)
	{

	}

	bool AssetDeserializer::ReadAndVerifyHeader()
	{
		char header[8];
		stream.Read(header, 8);
		return memcmp(header, ASSET_FILE_HEADER, ASSET_FILE_HEADER_LENGTH) == 0;
	}

	AssetType AssetDeserializer::ReadAssetType()
	{
		return stream.Read<AssetType>();
	}

	void AssetDeserializer::ReadSkeleton(Skeleton& outSkeleton)
	{
		const auto jointCount = stream.Read<unsigned>();
		outSkeleton.joints.resize(jointCount);
		for (auto& joint : outSkeleton.joints)
		{
			stream >> joint.inverseBindPose;
			stream >> joint.name;
			stream >> joint.parentIndex;
		}
	}

	void AssetDeserializer::ReadMesh(Mesh& outMesh)
	{
		const auto verticesCount = stream.Read<unsigned>();
		outMesh.m_vertices.resize(verticesCount);
		stream.Read(outMesh.m_vertices.data(), verticesCount);

		const auto indicesCount = stream.Read<unsigned>();
		outMesh.m_indices.resize(indicesCount);
		stream.Read(outMesh.m_indices.data(), indicesCount);

		stream >> outMesh.m_diffuseMapName;
		stream >> outMesh.m_specularMapName;
		stream >> outMesh.m_normalMapName;

		stream >> outMesh.m_diffuseIndex;
		stream >> outMesh.m_specularIndex;
		stream >> outMesh.m_normalIndex;
	}

	void AssetDeserializer::ReadAnimation(Animation& outAnimation)
	{
		stream >> outAnimation.name;

		ReadAnimationSQT(outAnimation.translations);
		ReadAnimationSQT(outAnimation.rotations);
		ReadAnimationSQT(outAnimation.scales);

		stream >> outAnimation.duration;
	}

	void AssetDeserializer::ReadTexture(Texture& outTexture)
	{
		stream >> outTexture.width;
		stream >> outTexture.height;
        stream >> outTexture.channels;

        // TODO: Should this be allocating memory?
        int size = outTexture.width * outTexture.height * outTexture.channels;
        outTexture.data = std::vector<std::byte>(size);
        stream.Read(outTexture.data.data(), size);
	}

	template <typename T>
	void AssetDeserializer::ReadAnimationSQT(std::vector<std::vector<T>>& outComponents)
	{
		const auto componentsCount = stream.Read<unsigned>();
		outComponents.resize(componentsCount);
		for (auto& components : outComponents)
		{
			const auto keyCount = stream.Read<unsigned>();
			components.resize(keyCount);
			stream.Read(components.data(), keyCount);
		}
	}
}
