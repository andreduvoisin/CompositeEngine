#ifndef _CE_ASSET_IMPORTER_H_
#define _CE_ASSET_IMPORTER_H_

#include <vector>

namespace CE
{
	struct Skeleton;
	struct Mesh;
	typedef std::vector<Mesh> Meshes;
	struct Animation;
	typedef std::vector<Animation> Animations;
	struct Texture;
	typedef std::vector<Texture> Textures;

	class AssetImporter
	{
	public:
		static bool ImportSkeletonMeshesAnimationsTextures(
			const char* fileName,
			Skeleton& outSkeleton,
			Meshes& outMeshes,
			Animations& outAnimations,
			Textures& outTextures);
	};
}

#endif // _CE_ASSET_IMPORTER_H_