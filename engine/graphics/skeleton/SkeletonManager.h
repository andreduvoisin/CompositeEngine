#ifndef _CE_SKELETON_MANAGER_H_
#define _CE_SKELETON_MANAGER_H_

#include "common/Singleton.h"

#include <string>
#include <unordered_map>

namespace CE
{
	struct Skeleton;
	class File3DImporter;

	class SkeletonManager : public Singleton<SkeletonManager>
	{
	public:
		void Initialize(File3DImporter* importer);
		void Destroy();

		Skeleton* GetSkeleton(const char* szSkeletonFile);

	private:
		File3DImporter* m_importer;
		std::unordered_map<std::string, Skeleton*> m_skeletonMap;
	};
}

#endif // _CE_SKELETON_MANAGER_H_
