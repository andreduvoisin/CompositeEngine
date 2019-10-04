#include "FBXUtilities.h"

#include <iostream>

namespace CE
{
    /* Tab character ("\t") counter */
    int numTabs = 0;

    /**
     * Print the required number of tabs.
     */
    void PrintTabs()
    {
        for (int i = 0; i < numTabs; i++)
        {
            std::cout << "\t";
        }
    }

    /**
     * Return a string-based representation based on the attribute type.
     */
    fbxsdk::FbxString GetAttributeTypeName(fbxsdk::FbxNodeAttribute::EType type)
    {
        switch (type)
        {
            case fbxsdk::FbxNodeAttribute::eUnknown:
                return "unidentified";
            case fbxsdk::FbxNodeAttribute::eNull:
                return "null";
            case fbxsdk::FbxNodeAttribute::eMarker:
                return "marker";
            case fbxsdk::FbxNodeAttribute::eSkeleton:
                return "skeleton";
            case fbxsdk::FbxNodeAttribute::eMesh:
                return "mesh";
            case fbxsdk::FbxNodeAttribute::eNurbs:
                return "nurbs";
            case fbxsdk::FbxNodeAttribute::ePatch:
                return "patch";
            case fbxsdk::FbxNodeAttribute::eCamera:
                return "camera";
            case fbxsdk::FbxNodeAttribute::eCameraStereo:
                return "stereo";
            case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
                return "camera switcher";
            case fbxsdk::FbxNodeAttribute::eLight:
                return "light";
            case fbxsdk::FbxNodeAttribute::eOpticalReference:
                return "optical reference";
            case fbxsdk::FbxNodeAttribute::eOpticalMarker:
                return "marker";
            case fbxsdk::FbxNodeAttribute::eNurbsCurve:
                return "nurbs curve";
            case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
                return "trim nurbs surface";
            case fbxsdk::FbxNodeAttribute::eBoundary:
                return "boundary";
            case fbxsdk::FbxNodeAttribute::eNurbsSurface:
                return "nurbs surface";
            case fbxsdk::FbxNodeAttribute::eShape:
                return "shape";
            case fbxsdk::FbxNodeAttribute::eLODGroup:
                return "lodgroup";
            case fbxsdk::FbxNodeAttribute::eSubDiv:
                return "subdiv";
            default:
                return "unknown";
        }
    }

    /**
     * Print an attribute.
     */
    void PrintAttribute(fbxsdk::FbxNodeAttribute* pAttribute)
    {
        if (pAttribute == nullptr)
        {
            return;
        }

        fbxsdk::FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
        fbxsdk::FbxString attrName = pAttribute->GetName();
        PrintTabs();
        // Note: to retrieve the character array of a fbxsdk::FbxString, use its Buffer() method.
        std::cout << "<attribute"
                  << " type='" << typeName.Buffer() << "'"
                  << " name='" << attrName.Buffer() << "'"
                  << "/>\n";
    }

    /**
     * Print a node, its attributes, and all its children recursively.
     */
    void PrintNode(fbxsdk::FbxNode* pNode)
    {
        PrintTabs();
        const char* nodeName = pNode->GetName();

        fbxsdk::FbxDouble3 translation = pNode->LclTranslation.Get();
        fbxsdk::FbxDouble3 rotation = pNode->LclRotation.Get();
        fbxsdk::FbxDouble3 scaling = pNode->LclScaling.Get();

        // Print the contents of the node.
        std::cout << "<node"
                  << " name='" << nodeName << "'"
                  << " translation='(" << translation[0] << ", " << translation[1] << ", " << translation[2] << ")'"
                  << " rotation='(" << rotation[0] << ", " << rotation[1] << ", " << rotation[2] << ")'"
                  << " scaling='(" << scaling[0] << ", " << scaling[1] << ", " << scaling[2] << ")'"
                  << ">\n";
        numTabs++;

        // Print the node's attributes.
        for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        {
            PrintAttribute(pNode->GetNodeAttributeByIndex(i));
        }

        // Recursively print the children.
        for (int j = 0; j < pNode->GetChildCount(); j++)
        {
            PrintNode(pNode->GetChild(j));
        }

        numTabs--;
        PrintTabs();
        std::cout << "</node>\n";
    }
}
