#include "2d/STBImageImporter.h"
#include "3d/AnimationOptimizer.h"
#include "3d/fbx/FBXImporter.h"
#include "graphics/animation/Animation.h"
#include "graphics/ceasset/output/AssetExporter.h"
#include "graphics/mesh/Mesh.h"
#include "graphics/skeleton/Skeleton.h"
#include "graphics/texture/Texture.h"

bool ConvertAsset(const std::string& fileName)
{
    CE::FBXImporter fbxImporter;
    CE::STBImageImporter stbImageImporter;

    printf("Converting %s...\n", fileName.c_str());

    printf("Extracting skeleton...\n");

    CE::Skeleton skeleton;
    if (!fbxImporter.ExtractSkeleton(fileName.c_str(), &skeleton))
    {
        printf("Extracting skeleton failed: %s\n", fileName.c_str());
        return false;
    }

    printf("Extracting meshes...\n");

    CE::Meshes meshes;
    if (!fbxImporter.ExtractMeshes(fileName.c_str(), skeleton, &meshes))
    {
        printf("Extracting meshes failed: %s\n", fileName.c_str());
        return false;
    }

    printf("Extracting animations...\n");

    CE::Animations animations;
    if (!fbxImporter.ExtractAnimations(fileName.c_str(), skeleton, &animations))
    {
        printf("Extracting animations failed: %s\n", fileName.c_str());
        return false;
    }

    printf("Extracting textures...\n");

    bool textureSuccess = true;
    CE::Textures textures;
    for (CE::Mesh& mesh : meshes)
    {
        textures.push_back(CE::Texture());
        CE::Texture& texture = textures.back();
        if (!stbImageImporter.ExtractTexture(mesh.m_diffuseMapName.c_str(), &texture))
        {
            printf("Extracting texture failed: %s\n", mesh.m_diffuseMapName.c_str());
            textureSuccess = false;
        }
        mesh.m_diffuseIndex = static_cast<uint8_t>(textures.size() - 1);
    }

    if (!textureSuccess)
    {
        printf("Extracting textures failed: %s\n", fileName.c_str());
        return false;
    }

    printf("Optimizing animations...\n");

    CE::AnimationOptimizer optimizer(&animations);
    optimizer.OptimizeAnimations();

    printf("Exporting ceasset file...\n");

    const auto position = fileName.find_last_of('.');
    std::string outputFileName = fileName.substr(0, position) + ".ceasset";

    bool exportSuccess = CE::AssetExporter::ExportSkeletonMeshesAnimationsTextures(
            outputFileName.c_str(), skeleton, meshes, animations, textures);
    if (!exportSuccess)
    {
        printf("Export failed: %s\n", fileName.c_str());
        return false;
    }

    printf("Exported: %s\n", outputFileName.c_str());
    return true;
}

int main(int argc, char* argv[])
{
    unsigned assetsExportedCount = 0;

    // TODO: Build argument parser.
    // For now, all args (except 0) are full paths to files.
    for (int argIndex = 1; argIndex < argc; ++argIndex)
    {
        std::string fileName = argv[argIndex];

        bool success = ConvertAsset(fileName);

        if (success)
        {
            ++assetsExportedCount;
        }
    }

    printf("Exported %u assets.\n", assetsExportedCount);

    printf("Done.\n");

    getchar();

    return 0;
}
