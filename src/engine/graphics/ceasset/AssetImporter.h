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

	class AssetImporter
	{
	public:
		static bool ImportSkeletonMeshesAnimationsTexture(
			const char* fileName,
			Skeleton& outSkeleton,
			Meshes& outMeshes,
			Animations& outAnimations,
			Texture& outTexture);
	};
}

#endif // _CE_ASSET_IMPORTER_H_