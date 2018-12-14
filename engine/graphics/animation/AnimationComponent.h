#ifndef _CE_ANIMATION_COMPONENT_H_
#define _CE_ANIMATION_COMPONENT_H_

#include "AnimationEventHandler.h"

#include <glm\glm.hpp>

#include <vector>

class EventSystem;
typedef unsigned int GLuint;

namespace CE
{
	struct Skeleton;
	struct Animation;
	typedef std::vector<Animation> Animations;

	struct AnimationCache
	{
		float currTime;
		std::vector<int> currTranslations;
		std::vector<int> currRotations;
		std::vector<int> currScales;
	};

	class AnimationComponent
	{
	public:
		AnimationComponent(
			Skeleton* skeleton,
			Animations* animations,
			EventSystem* eventSystem);

		void Update(float deltaSeconds);
		void BindMatrixPalette(
			GLuint g_paletteTextureUnit,
			GLuint g_paletteGenTex,
			GLuint g_tbo,
			GLuint g_paletteID);

		void ResetMatrixPalette();

		// TODO: Remove.
		const Skeleton* GetSkeleton() const { return m_skeleton; }

	private:
		void InitializeAnimationCache();
		void InitializePalette();
		void FindInterpolationKeys(size_t currentJoint);

		AnimationEventHandler animationEventHandler;

		Skeleton* m_skeleton;
		Animations* m_animations;
		std::vector<AnimationCache> m_animationCaches;
		std::vector<glm::mat4> m_palette;
		int m_currentAnimation;

		friend class AnimationEventHandler;
	};
}

#endif // _CE_ANIMATION_COMPONENT_H_