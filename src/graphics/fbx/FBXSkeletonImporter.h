#ifndef _CE_FBX_SKELETON_IMPORTER_H_
#define _CE_FBX_SKELETON_IMPORTER_H_

namespace fbxsdk
{
	class FbxManager;
	class FbxNode;
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
		void ProcessSkeletonHierarchy(fbxsdk::FbxNode* inRootNode);
		void ProcessSkeletonHierarchyRecursively(fbxsdk::FbxNode* inNode, int inDepth, int myIndex, int inParentIndex);

	private:
		fbxsdk::FbxManager* m_fbxManager;
		const char* m_szFileName;
		Skeleton* m_outSkeleton;
	};
}

#endif // _CE_FBX_SKELETON_IMPORTER_H_