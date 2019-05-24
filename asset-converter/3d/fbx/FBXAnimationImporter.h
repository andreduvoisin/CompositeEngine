#ifndef _CE_FBX_ANIMATION_IMPORTER_H_
#define _CE_FBX_ANIMATION_IMPORTER_H_

#include <vector>

namespace fbxsdk
{
	class FbxManager;
	class FbxNode;
	class FbxScene;
}

namespace CE
{
	struct Skeleton;
	struct Animation;
	typedef std::vector<Animation> Animations;

	class FBXAnimationImporter
	{
	public:
		FBXAnimationImporter(
			fbxsdk::FbxManager* fbxManager,
			const char* fileName,
			const Skeleton& skeleton,
			Animations* outAnimations);

		bool LoadAnimations();

	private:
		void ProcessAnimation(fbxsdk::FbxNode* node, fbxsdk::FbxScene* scene);

	private:
		fbxsdk::FbxManager* fbxManager;
		const char* fileName;
		const Skeleton& skeleton;
		Animations* outAnimations;
	};
}

#endif // _CE_FBX_ANIMATION_IMPORTER_H_