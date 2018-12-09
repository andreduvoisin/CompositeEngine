#include "AnimationComponent.h"

#include "Animation.h"
#include "graphics\skeleton\Skeleton.h"

#include "event/core/EventSystem.h"
#include "common\Math.h"

#include <GL\glew.h>

#include <algorithm>
#include "event/AnimationStateEvent.h"
#include "event/SetAnimationTimeEvent.h"

namespace CE
{
	AnimationComponent::AnimationComponent(
			Skeleton* skeleton,
			Animations* animations,
			EventSystem* eventSystem)
		: animationEventHandler(eventSystem, this)
		, m_skeleton(skeleton)
		, m_animations(animations)
		, m_currentAnimation(0)
	{
		InitializeAnimationCache();
		InitializePalette();
	}

	void AnimationComponent::InitializeAnimationCache()
	{
		m_animationCaches.reserve(m_animations->size());
		for (size_t i = 0; i < m_animations->size(); ++i)
		{
			AnimationCache animationCache;

			animationCache.currTime = 0;
			animationCache.currTranslations.resize(m_animations->at(i).translations.size(), 0);
			animationCache.currRotations.resize(m_animations->at(i).rotations.size(), 0);
			animationCache.currScales.resize(m_animations->at(i).scales.size(), 0);

			m_animationCaches.push_back(animationCache);
		}
	}

	void AnimationComponent::InitializePalette()
	{
		m_palette.reserve(m_skeleton->joints.size());
		for (size_t i = 0; i < m_skeleton->joints.size(); ++i)
		{
			m_palette.push_back(glm::mat4());
		}
	}

	void AnimationComponent::FindInterpolationKeys(int currentJoint)
	{
		Animation& animation = m_animations->at(m_currentAnimation);
		AnimationCache& animationCache = m_animationCaches[m_currentAnimation];

		// TODO: this can definitely be one function for all three.
		int& currTranslations = animationCache.currTranslations[currentJoint];
		const std::vector<TranslationKey>& translations = animation.translations[currentJoint];
		while (true)
		{
			if (translations.size() == 1)
			{
				currTranslations = 0;
				break;
			}

			if (translations[currTranslations].time > animationCache.currTime)
			{
				currTranslations = 0;
				continue;
			}

			if (translations[currTranslations].time <= animationCache.currTime
				&& translations[currTranslations + 1].time >= animationCache.currTime)
			{
				break;
			}

			++currTranslations;
		}

		// TODO: this can definitely be one function for all three.
		int& currRotations = animationCache.currRotations[currentJoint];
		const std::vector<RotationKey>& rotations = animation.rotations[currentJoint];
		while (true)
		{
			if (rotations.size() == 1)
			{
				currRotations = 0;
				break;
			}

			if (rotations[currRotations].time > animationCache.currTime)
			{
				currRotations = 0;
				continue;
			}

			if (rotations[currRotations].time <= animationCache.currTime
				&& rotations[currRotations + 1].time >= animationCache.currTime)
			{
				break;
			}

			++currRotations;
		}

		// TODO: this can definitely be one function for all three.
		int& currScales = animationCache.currScales[currentJoint];
		const std::vector<ScaleKey>& scales = animation.scales[currentJoint];
		while (true)
		{
			if (scales.size() == 1)
			{
				currScales = 0;
				break;
			}

			if (scales[currScales].time > animationCache.currTime)
			{
				currScales = 0;
				continue;
			}

			if (scales[currScales].time <= animationCache.currTime
				&& scales[currScales + 1].time >= animationCache.currTime)
			{
				break;
			}

			++currScales;
		}
	}

	void AnimationComponent::Update(float deltaSeconds)
	{
		if (m_animations->empty())
		{
			return;
		}

		Animation* animation = &m_animations->at(m_currentAnimation);
		AnimationCache* animationCache = &m_animationCaches[m_currentAnimation];

		animationCache->currTime += deltaSeconds;

		if (animationCache->currTime > animation->duration)
		{
			// use this to loop one animation
			//animationCache->currTime = fmod(animationCache->currTime, animation->duration);

			// use this to loop all animations
			animationCache->currTime = 0;
			m_currentAnimation = ++m_currentAnimation % m_animations->size();
			animation = &m_animations->at(m_currentAnimation);
			animationCache = &m_animationCaches[m_currentAnimation];
		}

		for (int i = 0; i < m_skeleton->joints.size(); ++i)
		{
			FindInterpolationKeys(i);

			const TranslationKey& lowTranslationKey = animation->translations[i][animationCache->currTranslations[i]];
			const TranslationKey& highTranslationKey = animation->translations[i][std::min(animationCache->currTranslations[i] + 1, (int)animation->translations[i].size() - 1)];
			const float translationAlpha = (animationCache->currTime - lowTranslationKey.time) / (highTranslationKey.time - lowTranslationKey.time);
			const glm::vec3 translation = LerpTranslation(
				lowTranslationKey.translation,
				highTranslationKey.translation,
				highTranslationKey.time == lowTranslationKey.time ? 0.f : translationAlpha);

			const RotationKey& lowRotationKey = animation->rotations[i][animationCache->currRotations[i]];
			const RotationKey& highRotationKey = animation->rotations[i][std::min(animationCache->currRotations[i] + 1, (int)animation->rotations[i].size() - 1)];
			const float rotationAlpha = (animationCache->currTime - lowRotationKey.time) / (highRotationKey.time - lowRotationKey.time);
			const glm::quat rotation = LerpRotation(
				lowRotationKey.rotation,
				highRotationKey.rotation,
				highRotationKey.time == lowRotationKey.time ? 0.f : rotationAlpha);

			const ScaleKey& lowScaleKey = animation->scales[i][animationCache->currScales[i]];
			const ScaleKey& highScaleKey = animation->scales[i][std::min(animationCache->currScales[i] + 1, (int)animation->scales[i].size() - 1)];
			const float scaleAlpha = (animationCache->currTime - lowScaleKey.time) / (highScaleKey.time - lowScaleKey.time);
			const glm::vec3 scale = LerpScale(
				lowScaleKey.scale,
				highScaleKey.scale,
				highScaleKey.time == lowScaleKey.time ? 0.f : scaleAlpha);

			glm::mat4 localPose = ToAffineMatrix(translation, rotation, scale);

			if (m_skeleton->joints[i].parentIndex == -1)
			{
				m_palette[i] = localPose;
			}
			else
			{
				m_palette[i] = m_palette[m_skeleton->joints[i].parentIndex] * localPose;
			}
		}

		for (int i = 0; i < m_skeleton->joints.size(); ++i)
		{
			m_palette[i] = m_palette[i] * m_skeleton->joints[i].inverseBindPose;
		}

		animationEventHandler.SendAnimationStateEvent();
	}

	void AnimationComponent::BindMatrixPalette(
		GLuint g_paletteTextureUnit,
		GLuint g_paletteGenTex,
		GLuint g_tbo,
		GLuint g_paletteID)
	{
		if (m_palette.empty())
		{
			return;
		}

		glActiveTexture(GL_TEXTURE0 + g_paletteTextureUnit);
		glBindTexture(GL_TEXTURE_BUFFER, g_paletteGenTex);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, g_tbo);
		glBindBuffer(GL_TEXTURE_BUFFER, g_tbo);
		glBufferData(GL_TEXTURE_BUFFER, m_palette.size() * sizeof(glm::mat4), m_palette.data(), GL_DYNAMIC_DRAW);
		glUniform1i(g_paletteID, g_paletteTextureUnit);
	}

	void AnimationComponent::ResetMatrixPalette()
	{
		for (int i = 0; i < m_skeleton->joints.size(); ++i)
		{
			m_palette[i] = glm::mat4(1.f);
		}
	}
}
