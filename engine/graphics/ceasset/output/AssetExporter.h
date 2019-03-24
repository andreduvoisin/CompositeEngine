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
	typedef std::vector<Texture> Textures;

	class AssetExporter
	{
	public:
		static bool ExportSkeletonMeshesAnimationsTextures(
			const char* fileName, 
			const Skeleton& skeleton, 
			const Meshes& meshes,
			const Animations& animations,
			const Textures& textures);
	};
}

#endif // _CE_ASSET_EXPORTER_H_