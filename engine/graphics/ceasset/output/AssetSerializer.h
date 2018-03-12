#ifndef _CE_ASSET_SERIALIZER_H_
#define _CE_ASSET_SERIALIZER_H_

#include <vector>

namespace CE
{
	class OutputFileStream;
	struct Skeleton;
	struct Mesh;
	typedef std::vector<Mesh> Meshes;
	struct Animation;
	typedef std::vector<Animation> Animations;
	struct Texture;

	class AssetSerializer
	{
	public:
		AssetSerializer(OutputFileStream& stream);
		~AssetSerializer() = default;
		AssetSerializer(const AssetSerializer&) = delete;
		AssetSerializer(AssetSerializer&& other) = delete;
		AssetSerializer& operator=(const AssetSerializer&) = delete;
		AssetSerializer& operator=(AssetSerializer&&) = delete;

		void WriteHeader();
		void WriteSkeleton(const Skeleton& skeleton);
		void WriteMesh(const Mesh& mesh);
		void WriteMeshes(const Meshes& meshes);
		void WriteAnimation(const Animation& animation);
		void WriteAnimations(const Animations& animations);
		void WriteTexture(const Texture& texture);

	private:
		template<typename T>
		void WriteAnimationSQT(const std::vector<std::vector<T>>& components);

	private:
		// TODO: Convert from reference to pointer?
		OutputFileStream& stream;
	};
}

#endif // _CE_ASSET_SERIALIZER_H_