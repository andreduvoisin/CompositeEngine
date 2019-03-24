#include "AssetExporter.h"

#include "AssetSerializer.h"
#include "OutputFileStream.h"


namespace CE
{
	bool AssetExporter::ExportSkeletonMeshesAnimationsTextures(
		const char* fileName,
		const Skeleton& skeleton,
		const Meshes& meshes,
		const Animations& animations,
		const Textures& textures)
	{
		OutputFileStream stream(fileName);

		if (!stream.IsValid())
		{
			return false;
		}

		AssetSerializer serializer(stream);
		serializer.WriteHeader();
		serializer.WriteSkeleton(skeleton);
		serializer.WriteMeshes(meshes);
		serializer.WriteAnimations(animations);
		serializer.WriteTextures(textures);

		return stream.IsValid();
	}
}
