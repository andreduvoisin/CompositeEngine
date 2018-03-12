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
			const char* szFileName,
			const Skeleton& skeleton,
			Animations* outAnimations);

		bool LoadAnimations();

	private:
		void ProcessAnimation(fbxsdk::FbxNode* node, fbxsdk::FbxScene* scene);

	private:
		fbxsdk::FbxManager* m_fbxManager;
		const char* m_szFileName;
		const Skeleton& m_skeleton;
		Animations* m_outAnimations;
	};
}

#endif // _CE_FBX_ANIMATION_IMPORTER_H_