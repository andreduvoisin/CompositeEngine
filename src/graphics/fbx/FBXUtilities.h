#ifndef _CE_FBX_UTILITIES_H_
#define _CE_FBX_UTILITIES_H_

#include <fbxsdk.h>

namespace CE
{
	void PrintTabs();
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
	void PrintAttribute(FbxNodeAttribute* pAttribute);
	void PrintNode(FbxNode* pNode);
}

#endif // _CE_FBX_UTILITIES_H_