#ifndef _CE_FBX_IMPORTER_H_
#define _CE_FBX_IMPORTER_H_

#include "graphics/File3DImporter.h"

// NOLINTNEXTLINE(readability-identifier-naming)
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

        FBXImporter(const FBXImporter&) = delete;
        FBXImporter(FBXImporter&&) = delete;
        FBXImporter& operator=(const FBXImporter&) = delete;
        FBXImporter& operator=(FBXImporter&&) = delete;

        bool ExtractSkeleton(const char* fileName, Skeleton* outSkeleton) override;
        bool ExtractMeshes(const char* fileName, const Skeleton& skeleton, Meshes* outMeshes) override;
        bool ExtractAnimations(const char* fileName, const Skeleton& skeleton, Animations* outAnimations) override;

    private:
        fbxsdk::FbxManager* fbxManager;
    };
}

#endif // _CE_FBX_IMPORTER_H_
