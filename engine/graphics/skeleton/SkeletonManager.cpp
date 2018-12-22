#include "SkeletonManager.h"

#include "Skeleton.h"
#include "graphics/File3DImporter.h"

namespace CE
{
	void SkeletonManager::Initialize(File3DImporter* importer)
	{
		m_importer = importer;
	}

	void SkeletonManager::Destroy()
	{
		for (auto it = m_skeletonMap.begin(); it != m_skeletonMap.end(); ++it)
		{
			delete it->second;
		}
		m_skeletonMap.clear();
	}

	Skeleton* SkeletonManager::GetSkeleton(const char* szSkeletonFile)
	{
		// Try to find the skeleton.
		auto it = m_skeletonMap.find(szSkeletonFile);

		// If it exists, return it.
		if (it != m_skeletonMap.end())
		{
			return it->second;
		}

		// Otherwise, attempt to load it.
		Skeleton* skeleton = new Skeleton();
		if (!m_importer->ExtractSkeleton(szSkeletonFile, skeleton))
		{
			delete skeleton;
			return nullptr;
		}
		m_skeletonMap[szSkeletonFile] = skeleton;
		return skeleton;
	}
}
