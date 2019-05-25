#ifndef _CE_FBX_SKELETON_IMPORTER_H_
#define _CE_FBX_SKELETON_IMPORTER_H_

#include <cstddef>

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
			const char* fileName,
			Skeleton* outSkeleton);

		bool LoadSkeleton();

	private:
		void ProcessSkeletonHierarchy(fbxsdk::FbxNode* rootNode, fbxsdk::FbxScene* scene);
		void ProcessSkeletonHierarchyRecursively(fbxsdk::FbxNode* node, size_t index, size_t parentIndex);
		bool JointHasChild(size_t index);
		void RemoveJoint(size_t index);

	private:
		fbxsdk::FbxManager* fbxManager;
		const char* fileName;
		Skeleton* outSkeleton;
	};
}

#endif // _CE_FBX_SKELETON_IMPORTER_H_
