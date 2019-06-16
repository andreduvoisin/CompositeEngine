#include "FBXAnimationImporter.h"

#include "FBXValidator.h"
#include "graphics/animation/Animation.h"
#include "graphics/skeleton/Skeleton.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <fbxsdk.h>

namespace CE
{
    const double ANIMATION_PERIOD = 1.F / 24.F;

    FBXAnimationImporter::FBXAnimationImporter(
            fbxsdk::FbxManager* fbxManager,
            const char* fileName,
            const Skeleton& skeleton,
            Animations* outAnimations)
        : fbxManager(fbxManager)
        , fileName(fileName)
        , skeleton(skeleton)
        , outAnimations(outAnimations)
    {
    }

    bool FBXAnimationImporter::LoadAnimations()
    {
        FBXValidator validator(fbxManager, fileName);
        if (!validator.Validate())
        {
            return false;
        }

        fbxsdk::FbxImporter* pImporter = fbxsdk::FbxImporter::Create(fbxManager, "");
        fbxsdk::FbxScene* pFbxScene = fbxsdk::FbxScene::Create(fbxManager, "");

        if (!pImporter->Initialize(fileName, -1, fbxManager->GetIOSettings()))
        {
            return false;
        }

        if (!pImporter->Import(pFbxScene))
        {
            return false;
        }

        pImporter->Destroy();

        fbxsdk::FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
        if (pFbxRootNode == nullptr)
        {
            return false;
        }

        ProcessAnimation(pFbxRootNode, pFbxScene);

        return true;
    }

    void FBXAnimationImporter::ProcessAnimation(fbxsdk::FbxNode* /*node*/, fbxsdk::FbxScene* scene)
    {
        const int animationCount = scene->GetSrcObjectCount<fbxsdk::FbxAnimStack>();
        outAnimations->resize(animationCount);

        for (int i = 0; i < animationCount; ++i)
        {
            Animation& animation = outAnimations->at(i);

            animation.translations.resize(skeleton.joints.size());
            animation.rotations.resize(skeleton.joints.size());
            animation.scales.resize(skeleton.joints.size());

            // Single-take animation information.
            auto* currAnimStack = scene->GetSrcObject<fbxsdk::FbxAnimStack>(i);
            scene->SetCurrentAnimationStack(currAnimStack);
            fbxsdk::FbxString animStackName = currAnimStack->GetName();
            std::string animationName = animStackName.Buffer();
            float start = (float) currAnimStack->GetLocalTimeSpan().GetStart().GetSecondDouble();
            float end = (float) currAnimStack->GetLocalTimeSpan().GetStop().GetSecondDouble();

            animation.name = animationName;
            animation.duration = end > start ? end - start : 1.F;

            fbxsdk::FbxAnimEvaluator* evaluator = scene->GetAnimationEvaluator();

            for (size_t j = 0; j < skeleton.joints.size(); ++j)
            {
                fbxsdk::FbxNode* joint = scene->FindNodeByName(skeleton.joints[j].name.c_str());

                // didn't find it, add bind pose
                if (joint == nullptr)
                {
                    // todo: bad
                    glm::vec3 scale;
                    glm::quat orientation;
                    glm::vec3 translation;
                    glm::vec3 skew;
                    glm::vec4 perspective;
                    glm::decompose(
                            glm::inverse(skeleton.joints[j].inverseBindPose),
                            scale,
                            orientation,
                            translation,
                            skew,
                            perspective);

                    // Translation.
                    TranslationKey translationKey;
                    translationKey.time = 0.F;
                    translationKey.translation = translation;
                    animation.translations[j].push_back(translationKey);

                    // Rotation.
                    RotationKey rotationKey;
                    rotationKey.time = 0.F;
                    rotationKey.rotation = orientation;
                    animation.rotations[j].push_back(rotationKey);

                    // Scale.
                    ScaleKey scaleKey;
                    scaleKey.time = 0.F;
                    scaleKey.scale = scale;
                    animation.scales[j].push_back(scaleKey);

                    continue;
                }

                // TODO: reserve/resize translations[j] rotations[j] scales[j]

                bool loop = true;
                double time = start;
                while (loop)
                {
                    if (time >= end)
                    {
                        loop = false;
                        time = end;
                    }

                    fbxsdk::FbxTime currTime = fbxsdk::FbxTimeSeconds(time);
                    // fbxsdk::FbxAMatrix currentTransformOffset = evaluator->GetNodeGlobalTransform(node, currTime) *
                    // geometryTransform; fbxsdk::FbxAMatrix localPose = currentTransformOffset.Inverse() *
                    // evaluator->GetNodeLocalTransform(joint, currTime);

                    fbxsdk::FbxAMatrix localPose = skeleton.joints[j].parentIndex == -1
                            ? evaluator->GetNodeGlobalTransform(joint, currTime)
                            : evaluator->GetNodeLocalTransform(joint, currTime);

                    auto frameTime = float(time - start);

                    // Translation.
                    fbxsdk::FbxVector4 translation = localPose.GetT();
                    TranslationKey translationKey;
                    translationKey.time = frameTime;
                    translationKey.translation[0] = (float) translation[0];
                    translationKey.translation[1] = (float) translation[1];
                    translationKey.translation[2] = (float) translation[2];
                    animation.translations[j].push_back(translationKey);

                    // Rotation.
                    fbxsdk::FbxQuaternion quaternion = localPose.GetQ();
                    RotationKey rotationKey;
                    rotationKey.time = frameTime;
                    rotationKey.rotation[0] = (float) quaternion[0];
                    rotationKey.rotation[1] = (float) quaternion[1];
                    rotationKey.rotation[2] = (float) quaternion[2];
                    rotationKey.rotation[3] = (float) quaternion[3];
                    animation.rotations[j].push_back(rotationKey);

                    // Scale.
                    fbxsdk::FbxVector4 scale = localPose.GetS();
                    ScaleKey scaleKey;
                    scaleKey.time = frameTime;
                    scaleKey.scale[0] = (float) scale[0];
                    scaleKey.scale[1] = (float) scale[1];
                    scaleKey.scale[2] = (float) scale[2];
                    animation.scales[j].push_back(scaleKey);

                    time += ANIMATION_PERIOD;
                }
            }
        }
    }
}
