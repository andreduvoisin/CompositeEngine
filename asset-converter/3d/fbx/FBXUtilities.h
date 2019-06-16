#ifndef _CE_FBX_UTILITIES_H_
#define _CE_FBX_UTILITIES_H_

#include <fbxsdk.h>

namespace CE
{
	void PrintTabs();
	fbxsdk::FbxString GetAttributeTypeName(fbxsdk::FbxNodeAttribute::EType type);
	void PrintAttribute(fbxsdk::FbxNodeAttribute* pAttribute);
	void PrintNode(fbxsdk::FbxNode* pNode);
}

#endif // _CE_FBX_UTILITIES_H_