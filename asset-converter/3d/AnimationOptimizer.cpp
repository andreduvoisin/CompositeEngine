#include "AnimationOptimizer.h"

#include "graphics\animation\Animation.h"
#include "common\Math.h"

#include <algorithm>

namespace CE
{
	AnimationOptimizer::AnimationOptimizer(Animations* animations)
		: m_animations(animations)
	{

	}

	void AnimationOptimizer::OptimizeAnimations()
	{
		for (size_t i = 0; i < m_animations->size(); ++i)
		{
			OptimizeAnimation((*m_animations)[i]);
		}
	}

	void AnimationOptimizer::OptimizeAnimation(Animation& animation)
	{
		// Prune animation.
		// TODO: This also needs to take into account how much it moves the entire heirarchy.
		float translationTolerance = 1e-3f; // 1 mm
		float rotationTolerance = .1f * glm::pi<float>() / 180.f; // 0.1 degree
		float scaleTolerance = 1e-3f; // 0.1%
		//float hierarchicalTolerance = 1e-3f; // 1 mm

		for (size_t i = 0; i < animation.translations.size(); ++i)
		{
			// Translation.
			// TODO: can these be done in place? is it even worth it?
			// TODO: remove duplication between s/r/t
			const std::vector<TranslationKey>& currentTranslations = animation.translations[i];
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

				const float alpha = (currentTranslations[j].time - first.time) / (currentTranslations[j + 1].time - first.time);
				glm::vec3 interpolated = LerpTranslation(first.translation, currentTranslations[j + 1].translation, alpha);
				// todo: use glm::length2
				if (std::abs(glm::length(currentTranslations[j].translation) - glm::length(interpolated)) > translationTolerance)
				{
					newTranslations.push_back(currentTranslations[j]);
				}
			}
			animation.translations[i] = newTranslations;
		}

		for (size_t i = 0; i < animation.rotations.size(); ++i)
		{
			// Rotation.
			// TODO: can these be done in place? is it even worth it?
			// TODO: remove duplication between s/r/t
			const std::vector<RotationKey>& currentRotations = animation.rotations[i];
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

				const float alpha = (currentRotations[j].time - first.time) / (currentRotations[j + 1].time - first.time);
				glm::quat interpolated = LerpRotation(first.rotation, currentRotations[j + 1].rotation, alpha);

				// TODO: HOW DOES THIS WORK?
				// Compute the shortest unsigned angle between the 2 quaternions.
				// diff_w is w component of a-1 * b.
				const float diff_w = interpolated.x * first.rotation.x
					+ interpolated.y * first.rotation.y
					+ interpolated.z * first.rotation.z
					+ interpolated.w * first.rotation.w;
				const float angle = 2.f * std::acos(std::min(std::abs(diff_w), 1.f));
				if (std::abs(angle) > rotationTolerance)
				{
					newRotations.push_back(currentRotations[j]);
				}
			}
			animation.rotations[i] = newRotations;
		}

		for (size_t i = 0; i < animation.scales.size(); ++i)
		{
			// Scale.
			// TODO: can these be done in place? is it even worth it?
			// TODO: remove duplication between s/r/t
			const std::vector<ScaleKey>& currentScales = animation.scales[i];
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
				if (std::abs(glm::length(currentScales[j].scale) - glm::length(interpolated)) > scaleTolerance)
				{
					newScales.push_back(currentScales[j]);
				}
			}
			animation.scales[i] = newScales;
		}
	}
}
