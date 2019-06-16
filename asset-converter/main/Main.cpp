#include "2d/STBImageImporter.h"
#include "3d/AnimationOptimizer.h"
#include "3d/fbx/FBXImporter.h"
#include "graphics/animation/Animation.h"
#include "graphics/ceasset/output/AssetExporter.h"
#include "graphics/mesh/Mesh.h"
#include "graphics/skeleton/Skeleton.h"
#include "graphics/texture/Texture.h"
#include <gsl/gsl>

#include <iostream>

bool ConvertAsset(const std::string& fileName)
{
    CE::FBXImporter fbxImporter;
    CE::STBImageImporter stbImageImporter;

    std::cout << "Converting " << fileName.c_str() << "...\n";

    std::cout << "Extracting skeleton...\n";

    CE::Skeleton skeleton;
    if (!fbxImporter.ExtractSkeleton(fileName.c_str(), &skeleton))
    {
        std::cout << "Extracting skeleton failed: " << fileName.c_str() << "\n";
        return false;
    }

    std::cout << "Extracting meshes...\n";

    CE::Meshes meshes;
    if (!fbxImporter.ExtractMeshes(fileName.c_str(), skeleton, &meshes))
    {
        std::cout << "Extracting meshes failed: " << fileName.c_str() << "\n";
        return false;
    }

    std::cout << "Extracting animations...\n";

    CE::Animations animations;
    if (!fbxImporter.ExtractAnimations(fileName.c_str(), skeleton, &animations))
    {
        std::cout << "Extracting animations failed: " << fileName.c_str() << "\n";
        return false;
    }

    std::cout << "Extracting textures...\n";

    bool textureSuccess = true;
    CE::Textures textures;
    for (CE::Mesh& mesh : meshes)
    {
        textures.push_back(CE::Texture());
        CE::Texture& texture = textures.back();
        if (!stbImageImporter.ExtractTexture(mesh.m_diffuseMapName.c_str(), &texture))
        {
            std::cout << "Extracting texture failed: " << mesh.m_diffuseMapName.c_str() << "\n";
            textureSuccess = false;
        }
        mesh.m_diffuseIndex = static_cast<uint8_t>(textures.size() - 1);
    }

    if (!textureSuccess)
    {
        std::cout << "Extracting textures failed: " << fileName.c_str() << "\n";
        return false;
    }

    std::cout << "Optimizing animations...\n";

    CE::AnimationOptimizer optimizer(&animations);
    optimizer.OptimizeAnimations();

    std::cout << "Exporting ceasset file...\n";

    const auto position = fileName.find_last_of('.');
    std::string outputFileName = fileName.substr(0, position) + ".ceasset";

    bool exportSuccess = CE::AssetExporter::ExportSkeletonMeshesAnimationsTextures(
            outputFileName.c_str(), skeleton, meshes, animations, textures);
    if (!exportSuccess)
    {
        std::cout << "Export failed: " << fileName.c_str() << "\n";
        return false;
    }

    std::cout << "Exported: " << outputFileName.c_str() << "\n";
    return true;
}

int main(int argc, char* argv[])
{
    // https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#slio10-unless-you-use-printf-family-functions-call-ios_basesync_with_stdiofalse
    std::ios_base::sync_with_stdio(false);

    // TODO: Build argument parser.
    // For now, all args (except 0) are full paths to files.
    gsl::multi_span<char*> arguments(argv, argc);

    unsigned assetsExportedCount = 0;

    auto it = std::begin(arguments);
    ++it; // Skip the first argument, which is the name of the executable.
    for (auto end = std::end(arguments); it != end; ++it)
    {
        bool success = ConvertAsset(*it);

        if (success)
        {
            ++assetsExportedCount;
        }
    }

    std::cout << "Exported " << assetsExportedCount << " assets.\n";

    std::cout << "Done.\n";

    getchar();

    return 0;
}
