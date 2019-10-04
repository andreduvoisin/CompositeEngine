#include "AnimationOptimizer.h"

#include "common/Math.h"
#include "graphics/animation/Animation.h"

#include <algorithm>

namespace CE
{
    const float DEFAULT_TRANSLATION_TOLERANCE = 1e-3F; // 1 mm
    const float DEFAULT_ROTATION_TOLERANCE = .1F * glm::pi<float>() / 180.F; // 0.1 degree
    const float DEFAULT_SCALE_TOLERANCE = 1e-3F; // 0.1%
    const float DEFAULT_HIERARCHICAL_TOLERANCE = 1e-3F; // 1 mm

    AnimationOptimizer::AnimationOptimizer(Animations* animations)
        : AnimationOptimizer(
                animations,
                DEFAULT_TRANSLATION_TOLERANCE,
                DEFAULT_ROTATION_TOLERANCE,
                DEFAULT_SCALE_TOLERANCE,
                DEFAULT_HIERARCHICAL_TOLERANCE)
    {
    }

    AnimationOptimizer::AnimationOptimizer(
            Animations* animations,
            float translationTolerance,
            float rotationTolerance,
            float scaleTolerance,
            float hierarchicalTolerance)
        : animations(animations)
        , TRANSLATION_TOLERANCE(translationTolerance)
        , ROTATION_TOLERANCE(rotationTolerance)
        , SCALE_TOLERANCE(scaleTolerance)
        , HIERARCHICAL_TOLERANCE(hierarchicalTolerance)
    {
    }

    void AnimationOptimizer::OptimizeAnimations()
    {
        for (auto& animation : *animations)
        {
            OptimizeAnimation(animation);
        }
    }

    // Prune animation.
    // TODO: This needs to take into account how much it moves the entire hierarchy.
    void AnimationOptimizer::OptimizeAnimation(Animation& animation)
    {
        for (auto& translation : animation.translations)
        {
            // Translation.
            // TODO: can these be done in place? is it even worth it?
            // TODO: remove duplication between s/r/t
            const std::vector<TranslationKey>& currentTranslations = translation;
            std::vector<TranslationKey> newTranslations;
            for (size_t j = 0; j < currentTranslations.size(); ++j)
            {
                // Keep first always.
                if (j == 0)
                {
                    newTranslations.push_back(currentTranslations[j]);
                    continue;
                }

                // Keep last always.
                if (j == currentTranslations.size() - 1)
                {
                    newTranslations.push_back(currentTranslations[j]);
                    break;
                }

                const TranslationKey& first = newTranslations[newTranslations.size() - 1];

                const float alpha =
                        (currentTranslations[j].time - first.time) / (currentTranslations[j + 1].time - first.time);
                glm::vec3 interpolated =
                        LerpTranslation(first.translation, currentTranslations[j + 1].translation, alpha);
                // todo: use glm::length2
                const float drift =
                        std::abs(glm::length(currentTranslations[j].translation) - glm::length(interpolated));
                if (drift > TRANSLATION_TOLERANCE)
                {
                    newTranslations.push_back(currentTranslations[j]);
                }
            }
            translation = newTranslations;
        }

        for (auto& rotation : animation.rotations)
        {
            // Rotation.
            // TODO: can these be done in place? is it even worth it?
            // TODO: remove duplication between s/r/t
            const std::vector<RotationKey>& currentRotations = rotation;
            std::vector<RotationKey> newRotations;
            for (size_t j = 0; j < currentRotations.size(); ++j)
            {
                // Keep first always.
                if (j == 0)
                {
                    newRotations.push_back(currentRotations[j]);
                    continue;
                }

                // Keep last always.
                if (j == currentRotations.size() - 1)
                {
                    newRotations.push_back(currentRotations[j]);
                    break;
                }

                const RotationKey& first = newRotations[newRotations.size() - 1];

                const float alpha =
                        (currentRotations[j].time - first.time) / (currentRotations[j + 1].time - first.time);
                glm::quat interpolated = LerpRotation(first.rotation, currentRotations[j + 1].rotation, alpha);

                // TODO: HOW DOES THIS WORK?
                // Compute the shortest unsigned angle between the 2 quaternions.
                // diffW is w component of a-1 * b.
                const float diffW = interpolated.x * first.rotation.x + interpolated.y * first.rotation.y
                        + interpolated.z * first.rotation.z + interpolated.w * first.rotation.w;
                const float angle = 2.F * std::acos(std::min(std::abs(diffW), 1.F));
                if (std::abs(angle) > ROTATION_TOLERANCE)
                {
                    newRotations.push_back(currentRotations[j]);
                }
            }
            rotation = newRotations;
        }

        for (auto& scale : animation.scales)
        {
            // Scale.
            // TODO: can these be done in place? is it even worth it?
            // TODO: remove duplication between s/r/t
            const std::vector<ScaleKey>& currentScales = scale;
            std::vector<ScaleKey> newScales;
            for (size_t j = 0; j < currentScales.size(); ++j)
            {
                // Keep first always.
                if (j == 0)
                {
                    newScales.push_back(currentScales[j]);
                    continue;
                }

                // Keep last always.
                if (j == currentScales.size() - 1)
                {
                    newScales.push_back(currentScales[j]);
                    break;
                }

                const ScaleKey& first = newScales[newScales.size() - 1];

                const float alpha = (currentScales[j].time - first.time) / (currentScales[j + 1].time - first.time);
                glm::vec3 interpolated = LerpScale(first.scale, currentScales[j + 1].scale, alpha);
                // todo: use glm::length2
                if (std::abs(glm::length(currentScales[j].scale) - glm::length(interpolated)) > SCALE_TOLERANCE)
                {
                    newScales.push_back(currentScales[j]);
                }
            }
            scale = newScales;
        }
    }
}
