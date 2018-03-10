#ifndef _CE_ASSET_EXPORTER_H_
#define _CE_ASSET_EXPORTER_H_

#include <vector>

namespace CE
{
	struct Skeleton;
	struct Mesh;
	typedef std::vector<Mesh> Meshes;
	struct Animation;
	typedef std::vector<Animation> Animations;
	struct Texture;

	class AssetExporter
	{
	public:
		static bool ExportSkeletonMeshesAnimationsTexture(
			const char* fileName, 
			const Skeleton& skeleton, 
			const Meshes& meshes,
			const Animations& animations,
			const Texture& texture);
	};
}

#endif // _CE_ASSET_EXPORTER_H_