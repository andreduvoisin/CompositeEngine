#include "2d/STBImageImporter.h"
#include "3d/fbx/FBXImporter.h"
#include "3d/AnimationOptimizer.h"

#include "graphics/ceasset/output/AssetExporter.h"

#include "graphics/skeleton/Skeleton.h"
#include "graphics/mesh/Mesh.h"
#include "graphics/animation/Animation.h"
#include "graphics/texture/Texture.h"

int main(int argc, char* argv[])
{
	// TODO: Build argument parser.
	// For now, all args (except 0) are full paths to files.
	unsigned assetsExportedCount = 0;

	CE::FBXImporter fbxImporter;
	CE::STBImageImporter stbImageImporter;

	for (int argIndex = 1; argIndex < argc; ++argIndex)
	{
		std::string fileName = argv[argIndex];

		printf("Converting %s...\n", fileName.c_str());

		printf("Extracting skeleton...\n");

		CE::Skeleton skeleton;
		if (!fbxImporter.ExtractSkeleton(fileName.c_str(), &skeleton))
		{
			printf("Extracting skeleton failed: %s\n", fileName.c_str());
			continue;
		}

		printf("Extracting meshes...\n");

		CE::Meshes meshes;
		if (!fbxImporter.ExtractMeshes(fileName.c_str(), skeleton, &meshes))
		{
			printf("Extracting meshes failed: %s\n", fileName.c_str());
			continue;
		}

		printf("Extracting animations...\n");

		CE::Animations animations;
		if (!fbxImporter.ExtractAnimations(fileName.c_str(), skeleton, &animations))
		{
			printf("Extracting animations failed: %s\n", fileName.c_str());
			continue;
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
			continue;
		}

		printf("Optimizing animations...\n");

		CE::AnimationOptimizer optimizer(&animations);
		optimizer.OptimizeAnimations();

		printf("Exporting ceasset file...\n");

		const auto position = fileName.find_last_of('.');
		std::string outputFileName = fileName.substr(0, position) + ".ceasset";

		bool exportSuccess = CE::AssetExporter::ExportSkeletonMeshesAnimationsTextures(
			outputFileName.c_str(),
			skeleton,
			meshes,
			animations,
			textures);
		if (exportSuccess)
		{
			printf("Exported: %s\n", outputFileName.c_str());
			++assetsExportedCount;
		}
		else
		{
			printf("Export failed: %s\n", fileName.c_str());
		}
	}

	printf("Exported %u assets.\n", assetsExportedCount);

	printf("Done.\n");

	getchar();

	return 0;
}
