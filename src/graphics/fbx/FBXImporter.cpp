#include "FBXImporter.h"

#include "FBXSkeletonImporter.h"
#include "FBXMeshImporter.h"
#include "FBXAnimationImporter.h"

#include <fbxsdk.h>

namespace CE
{
	FBXImporter::FBXImporter()
	{
		m_fbxManager = FbxManager::Create();

		FbxIOSettings* ioSettings = FbxIOSettings::Create(m_fbxManager, IOSROOT);
		m_fbxManager->SetIOSettings(ioSettings);
	}

	FBXImporter::~FBXImporter()
	{
		m_fbxManager->Destroy();
	}

	bool FBXImporter::ExtractSkeleton(const char* fileName, Skeleton* outSkeleton)
	{
		FBXSkeletonImporter skeletonImporter = FBXSkeletonImporter(m_fbxManager, fileName, outSkeleton);
		return skeletonImporter.LoadSkeleton();
	}

	bool FBXImporter::ExtractMeshes(const char* fileName, Skeleton& skeleton, Meshes* outMeshes)
	{
		FBXMeshImporter meshImporter = FBXMeshImporter(m_fbxManager, fileName, skeleton, outMeshes);
		return meshImporter.LoadMeshes();
	}

	bool FBXImporter::ExtractAnimations(const char* fileName, const Skeleton& skeleton, Animations* outAnimations)
	{
		FBXAnimationImporter animationImporter = FBXAnimationImporter(m_fbxManager, fileName, skeleton, outAnimations);
		return animationImporter.LoadAnimations();
	}
}
