#ifndef _CE_FBX_VALIDATOR_H_
#define _CE_FBX_VALIDATOR_H_

namespace fbxsdk
{
	class FbxManager;
	class FbxNode;
}

namespace CE
{
	class FBXValidator
	{
	public:
		FBXValidator(
			fbxsdk::FbxManager* fbxManager,
			const char* fileName);

		bool Validate();

	private:
		bool ValidateNode(fbxsdk::FbxNode* node);
		bool ValidateGeometry(fbxsdk::FbxNode* node);
		bool ValidateCluster(fbxsdk::FbxNode* node);
		bool ValidateChildren(fbxsdk::FbxNode* node);

	private:
		fbxsdk::FbxManager* fbxManager;
		const char* fileName;
	};
}

#endif // _CE_FBX_VALIDATOR_H_