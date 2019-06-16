#include "FBXImporter.h"

#include "FBXAnimationImporter.h"
#include "FBXMeshImporter.h"
#include "FBXSkeletonImporter.h"

#include <fbxsdk.h>

namespace CE
{
    FBXImporter::FBXImporter()
    {
        fbxManager = fbxsdk::FbxManager::Create();

        fbxsdk::FbxIOSettings* ioSettings = fbxsdk::FbxIOSettings::Create(fbxManager, IOSROOT);
        fbxManager->SetIOSettings(ioSettings);
    }

    FBXImporter::~FBXImporter()
    {
        fbxManager->Destroy();
    }

    bool FBXImporter::ExtractSkeleton(const char* fileName, Skeleton* outSkeleton)
    {
        FBXSkeletonImporter skeletonImporter = FBXSkeletonImporter(fbxManager, fileName, outSkeleton);
        return skeletonImporter.LoadSkeleton();
    }

    bool FBXImporter::ExtractMeshes(const char* fileName, const Skeleton& skeleton, Meshes* outMeshes)
    {
        FBXMeshImporter meshImporter = FBXMeshImporter(fbxManager, fileName, skeleton, outMeshes);
        return meshImporter.LoadMeshes();
    }

    bool FBXImporter::ExtractAnimations(const char* fileName, const Skeleton& skeleton, Animations* outAnimations)
    {
        FBXAnimationImporter animationImporter = FBXAnimationImporter(fbxManager, fileName, skeleton, outAnimations);
        return animationImporter.LoadAnimations();
    }
}
