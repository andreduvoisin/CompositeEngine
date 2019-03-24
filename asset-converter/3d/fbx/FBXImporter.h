#ifndef _CE_FBX_IMPORTER_H_
#define _CE_FBX_IMPORTER_H_

#include "graphics/File3DImporter.h"

namespace fbxsdk
{
	class FbxManager;
}

namespace CE
{
	class FBXImporter : public File3DImporter
	{
	public:
		FBXImporter();
		~FBXImporter();

		bool ExtractSkeleton(const char* fileName, Skeleton* outSkeleton);
		bool ExtractMeshes(const char* fileName, const Skeleton& skeleton, Meshes* outMeshes);
		bool ExtractAnimations(const char* fileName, const Skeleton& skeleton, Animations* outAnimations);

	private:
		fbxsdk::FbxManager* m_fbxManager;
	};
}

#endif // _CE_FBX_IMPORTER_H_
