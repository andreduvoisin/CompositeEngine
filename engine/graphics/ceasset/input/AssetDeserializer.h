#ifndef _CE_ASSET_DESERIALIZER_H_
#define _CE_ASSET_DESERIALIZER_H_

#include "graphics/ceasset/AssetTraits.h"

#include <vector>

namespace CE
{
	class InputFileStream;
	struct Skeleton;
	struct Mesh;
	struct Animation;
	struct Texture;

	class AssetDeserializer
	{
	public:
		AssetDeserializer(InputFileStream& stream);
		~AssetDeserializer() = default;
		AssetDeserializer(const AssetDeserializer&) = delete;
		AssetDeserializer(AssetDeserializer&& other) = delete;
		AssetDeserializer& operator=(const AssetDeserializer&) = delete;
		AssetDeserializer& operator=(AssetDeserializer&&) = delete;

		// TODO: Convert from reference to pointer?
		bool ReadAndVerifyHeader();
		AssetType ReadAssetType();
		void ReadSkeleton(Skeleton& outSkeleton);
		void ReadMesh(Mesh& outMesh);
		void ReadAnimation(Animation& outAnimation);
		void ReadTexture(Texture& outTexture);

	private:
		template<typename T>
		void ReadAnimationSQT(std::vector<std::vector<T>>& outComponents);

	private:
		// TODO: Convert from reference to pointer?
		InputFileStream& stream;
	};
}

#endif // _CE_ASSET_DESERIALIZER_H_
