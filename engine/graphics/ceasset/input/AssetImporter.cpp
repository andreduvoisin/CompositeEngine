#include "AssetImporter.h"

#include "AssetDeserializer.h"
#include "InputFileStream.h"

#include "graphics/mesh/Mesh.h"
#include "graphics/animation/Animation.h"
#include "graphics/texture/Texture.h"

namespace CE
{
	// TODO: Convert from reference to pointer?
	bool AssetImporter::ImportSkeletonMeshesAnimationsTextures(
		const char* fileName, 
		Skeleton& outSkeleton, 
		Meshes& outMeshes,
		Animations& outAnimations, 
		Textures& outTextures)
	{
		InputFileStream stream(fileName);

		if (!stream.IsValid())
		{
			return false;
		}

		AssetDeserializer deserializer(stream);

		if (!deserializer.ReadAndVerifyHeader())
		{
			return false;
		}

		while (stream.HasData())
		{
			switch (deserializer.ReadAssetType())
			{
				case AssetType::SKELETON:
					deserializer.ReadSkeleton(outSkeleton);
					break;

				case AssetType::MESH:
					outMeshes.push_back(Mesh());
					deserializer.ReadMesh(outMeshes.back());
					break;

				case AssetType::ANIMATION:
					outAnimations.push_back(Animation());
					deserializer.ReadAnimation(outAnimations.back());
					break;

				case AssetType::TEXTURE:
					outTextures.push_back(Texture());
					deserializer.ReadTexture(outTextures.back());
					break;
			}
		}

		return stream.IsValid();
	}
}
