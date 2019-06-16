#ifndef _CE_FBX_VALIDATOR_H_
#define _CE_FBX_VALIDATOR_H_

// NOLINTNEXTLINE(readability-identifier-naming)
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
        FBXValidator(fbxsdk::FbxManager* manager, const char* fileName);

        bool Validate();

    private:
        bool ValidateNode(fbxsdk::FbxNode* node);
        bool ValidateGeometry(fbxsdk::FbxNode* node);
        bool ValidateCluster(fbxsdk::FbxNode* node);
        bool ValidateChildren(fbxsdk::FbxNode* node);

        fbxsdk::FbxManager* manager;
        const char* fileName;
    };
}

#endif // _CE_FBX_VALIDATOR_H_