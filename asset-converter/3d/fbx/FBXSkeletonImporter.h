#ifndef _CE_FBX_SKELETON_IMPORTER_H_
#define _CE_FBX_SKELETON_IMPORTER_H_

namespace fbxsdk
{
	class FbxManager;
	class FbxNode;
	class FbxScene;
}

namespace CE
{
	struct Skeleton;

	class FBXSkeletonImporter
	{
	public:
		FBXSkeletonImporter(
			fbxsdk::FbxManager* fbxManager,
			const char* szFileName,
			Skeleton* outSkeleton);

		bool LoadSkeleton();

	private:
		void ProcessSkeletonHierarchy(fbxsdk::FbxNode* inRootNode, fbxsdk::FbxScene* pFbxScene);
		void ProcessSkeletonHierarchyRecursively(fbxsdk::FbxNode* inNode, size_t myIndex, size_t inParentIndex);
		bool JointHasChild(size_t index);
		void RemoveJoint(size_t index);

	private:
		fbxsdk::FbxManager* m_fbxManager;
		const char* m_szFileName;
		Skeleton* m_outSkeleton;
	};
}

#endif // _CE_FBX_SKELETON_IMPORTER_H_