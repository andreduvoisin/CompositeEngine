#ifndef _CE_ANIMATION_COMPONENT_H_
#define _CE_ANIMATION_COMPONENT_H_

#include <glm\glm.hpp>

#include <vector>

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
		AnimationComponent(Skeleton* skeleton, Animations* animations);

		void Update(float deltaTime);
		void BindMatrixPalette(
			GLuint g_paletteTextureUnit,
			GLuint g_paletteGenTex,
			GLuint g_tbo,
			GLuint g_paletteID);

	private:
		void InitializeAnimationCache();
		void InitializePalette();
		void FindInterpolationKeys(int currentJoint);

	private:
		Skeleton* m_skeleton;
		Animations* m_animations;

		std::vector<AnimationCache> m_animationCaches;
		std::vector<glm::mat4> m_palette;

		int m_currentAnimation;
	};
}

#endif // _CE_ANIMATION_COMPONENT_H_