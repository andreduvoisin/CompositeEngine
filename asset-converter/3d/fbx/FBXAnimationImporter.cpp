#include "FBXAnimationImporter.h"

#include "FBXValidator.h"
#include "graphics\animation\Animation.h"
#include "graphics\skeleton\Skeleton.h"

#include <fbxsdk.h>

#include <glm\glm.hpp>
#include <glm\gtx\matrix_decompose.hpp>

namespace CE
{
	FBXAnimationImporter::FBXAnimationImporter(
			FbxManager* fbxManager,
			const char* szFileName,
			const Skeleton& skeleton,
			Animations* outAnimations)
		: m_fbxManager(fbxManager)
		, m_szFileName(szFileName)
		, m_skeleton(skeleton)
		, m_outAnimations(outAnimations)
	{

	}

	bool FBXAnimationImporter::LoadAnimations()
	{
		FBXValidator validator(m_fbxManager, m_szFileName);
		if (!validator.Validate())
		{
			return false;
		}

		FbxImporter* pImporter = FbxImporter::Create(m_fbxManager, "");
		FbxScene* pFbxScene = FbxScene::Create(m_fbxManager, "");

		if (!pImporter->Initialize(m_szFileName, -1, m_fbxManager->GetIOSettings()))
		{
			return false;
		}

		if (!pImporter->Import(pFbxScene))
		{
			return false;
		}

		pImporter->Destroy();

		FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
		if (!pFbxRootNode)
		{
			return false;
		}

		ProcessAnimation(pFbxRootNode, pFbxScene);

		return true;
	}

	void FBXAnimationImporter::ProcessAnimation(FbxNode* node, FbxScene* scene)
	{
		const int animationCount = scene->GetSrcObjectCount<FbxAnimStack>();
		m_outAnimations->resize(animationCount);

		for (int i = 0; i < animationCount; ++i)
		{
			Animation& animation = m_outAnimations->at(i);

			animation.translations.resize(m_skeleton.joints.size());
			animation.rotations.resize(m_skeleton.joints.size());
			animation.scales.resize(m_skeleton.joints.size());

			// Single-take animation information.
			FbxAnimStack* currAnimStack = scene->GetSrcObject<FbxAnimStack>(i);
			scene->SetCurrentAnimationStack(currAnimStack);
			FbxString animStackName = currAnimStack->GetName();
			std::string animationName = animStackName.Buffer();
			float start = (float) currAnimStack->GetLocalTimeSpan().GetStart().GetSecondDouble();
			float end = (float) currAnimStack->GetLocalTimeSpan().GetStop().GetSecondDouble();

			animation.name = animationName;
			animation.duration = end > start ? end - start : 1.f;

			FbxAnimEvaluator* evaluator = scene->GetAnimationEvaluator();

			double period = 1.f / 24.f; // todo: make variable, it's all over this file

			for (int j = 0; j < m_skeleton.joints.size(); ++j)
			{
				FbxNode* joint = scene->FindNodeByName(m_skeleton.joints[j].name.c_str());

				// didn't find it, add bind pose
				if (!joint)
				{
					// todo: bad
					glm::vec3 scale;
					glm::quat orientation;
					glm::vec3 translation;
					glm::vec3 skew;
					glm::vec4 perspective;
					glm::decompose(
						glm::inverse(m_skeleton.joints[j].inverseBindPose),
						scale,
						orientation,
						translation,
						skew,
						perspective);

					// Translation.
					TranslationKey translationKey;
					translationKey.time = 0.f;
					translationKey.translation = translation;
					animation.translations[j].push_back(translationKey);

					// Rotation.
					RotationKey rotationKey;
					rotationKey.time = 0.f;
					rotationKey.rotation = orientation;
					animation.rotations[j].push_back(rotationKey);

					// Scale.
					ScaleKey scaleKey;
					scaleKey.time = 0.f;
					scaleKey.scale = scale;
					animation.scales[j].push_back(scaleKey);

					continue;
				}

				// TODO: reserve/resize translations[j] rotations[j] scales[j]

				bool loop = true;
				for (double time = start; loop; time += period)
				{
					if (time >= end)
					{
						loop = false;
						time = end;
					}

					FbxTime currTime = FbxTimeSeconds(time);
					//FbxAMatrix currentTransformOffset = evaluator->GetNodeGlobalTransform(node, currTime) * geometryTransform;
					//FbxAMatrix localPose = currentTransformOffset.Inverse() * evaluator->GetNodeLocalTransform(joint, currTime);

					FbxAMatrix localPose =
						m_skeleton.joints[j].parentIndex == -1 ?
						evaluator->GetNodeGlobalTransform(joint, currTime) :
						evaluator->GetNodeLocalTransform(joint, currTime);

					float frameTime = float(time - start);

					// Translation.
					FbxVector4 translation = localPose.GetT();
					TranslationKey translationKey;
					translationKey.time = frameTime;
					translationKey.translation[0] = (float) translation[0];
					translationKey.translation[1] = (float) translation[1];
					translationKey.translation[2] = (float) translation[2];
					animation.translations[j].push_back(translationKey);

					// Rotation.
					FbxQuaternion quaternion = localPose.GetQ();
					RotationKey rotationKey;
					rotationKey.time = frameTime;
					rotationKey.rotation[0] = (float) quaternion[0];
					rotationKey.rotation[1] = (float) quaternion[1];
					rotationKey.rotation[2] = (float) quaternion[2];
					rotationKey.rotation[3] = (float) quaternion[3];
					animation.rotations[j].push_back(rotationKey);

					// Scale.
					FbxVector4 scale = localPose.GetS();
					ScaleKey scaleKey;
					scaleKey.time = frameTime;
					scaleKey.scale[0] = (float) scale[0];
					scaleKey.scale[1] = (float) scale[1];
					scaleKey.scale[2] = (float) scale[2];
					animation.scales[j].push_back(scaleKey);
				}
			}
		}
	}
}
