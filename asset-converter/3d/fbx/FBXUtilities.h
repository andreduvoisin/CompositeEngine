#ifndef _CE_FBX_UTILITIES_H_
#define _CE_FBX_UTILITIES_H_

#include <fbxsdk.h>

namespace CE
{
	void PrintTabs();
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
	void PrintAttribute(FbxNodeAttribute* attribute);
	void PrintNode(FbxNode* node);
}

#endif // _CE_FBX_UTILITIES_H_