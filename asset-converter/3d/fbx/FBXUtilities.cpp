#include "FBXUtilities.h"

namespace CE
{
	/* Tab character ("\t") counter */
	int tabCount = 0;

	void PrintTabs()
	{
		for (int i = 0; i < tabCount; ++i)
		{
			printf("\t");
		}
	}

	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type)
	{
		switch (type)
		{
			case FbxNodeAttribute::eUnknown:
				return "unidentified";

			case FbxNodeAttribute::eNull:
				return "null";

			case FbxNodeAttribute::eMarker:
				return "marker";

			case FbxNodeAttribute::eSkeleton:
				return "skeleton";

			case FbxNodeAttribute::eMesh:
				return "mesh";

			case FbxNodeAttribute::eNurbs:
				return "nurbs";

			case FbxNodeAttribute::ePatch:
				return "patch";

			case FbxNodeAttribute::eCamera:
				return "camera";

			case FbxNodeAttribute::eCameraStereo:
				return "stereo";

			case FbxNodeAttribute::eCameraSwitcher:
				return "camera switcher";

			case FbxNodeAttribute::eLight:
				return "light";

			case FbxNodeAttribute::eOpticalReference:
				return "optical reference";

			case FbxNodeAttribute::eOpticalMarker:
				return "marker";

			case FbxNodeAttribute::eNurbsCurve:
				return "nurbs curve";

			case FbxNodeAttribute::eTrimNurbsSurface:
				return "trim nurbs surface";

			case FbxNodeAttribute::eBoundary:
				return "boundary";

			case FbxNodeAttribute::eNurbsSurface:
				return "nurbs surface";

			case FbxNodeAttribute::eShape:
				return "shape";

			case FbxNodeAttribute::eLODGroup:
				return "lodgroup";

			case FbxNodeAttribute::eSubDiv:
				return "subdiv";

			default:
				return "unknown";
		}
	}

	void PrintAttribute(FbxNodeAttribute* attribute)
	{
		if (!attribute)
		{
			return;
		}

		FbxString typeName = GetAttributeTypeName(attribute->GetAttributeType());
		FbxString attributeName = attribute->GetName();

		PrintTabs();

		printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attributeName.Buffer());
	}

	void PrintNode(FbxNode* node)
	{
		PrintTabs();

		const char* nodeName = node->GetName();
		FbxDouble3 translation = node->LclTranslation.Get();
		FbxDouble3 rotation = node->LclRotation.Get();
		FbxDouble3 scale = node->LclScaling.Get();

		printf(
			"<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scale='(%f, %f, %f)'>\n",
			nodeName,
			translation[0], translation[1], translation[2],
			rotation[0], rotation[1], rotation[2],
			scale[0], scale[1], scale[2]);

		++tabCount;

		for (int attributeIndex = 0; attributeIndex < node->GetNodeAttributeCount(); ++attributeIndex)
		{
			PrintAttribute(node->GetNodeAttributeByIndex(attributeIndex));
		}

		// Recursively print the children.
		for (int childIndex = 0; childIndex < node->GetChildCount(); ++childIndex)
		{
			PrintNode(node->GetChild(childIndex));
		}

		--tabCount;

		PrintTabs();

		printf("</node>\n");
	}
}
