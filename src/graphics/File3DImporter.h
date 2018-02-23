#ifndef _CE_FILE_3D_IMPORTER_H_
#define _CE_FILE_3D_IMPORTER_H_

#include <vector>

namespace CE
{
	struct Skeleton;
	struct Mesh;
	typedef std::vector<Mesh> Meshes;
	struct Animation;
	typedef std::vector<Animation> Animations;

	class File3DImporter
	{
	public:
		virtual bool ExtractSkeleton(const char* fileName, Skeleton* outSkeleton) = 0;
		virtual bool ExtractMeshes(const char* fileName, Skeleton& skeleton, Meshes* outMeshes) = 0;
		virtual bool ExtractAnimations(const char* fileName, const Skeleton& skeleton, Animations* outAnimations) = 0;
	};
}

#endif // _CE_FILE_3D_IMPORTER_H_