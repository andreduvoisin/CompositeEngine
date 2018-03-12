#ifndef _CE_ANIMATION_H_
#define _CE_ANIMATION_H_

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

#include <vector>

namespace CE
{
	struct TranslationKey
	{
		glm::vec3 translation;
		float time;
	};

	struct RotationKey
	{
		glm::quat rotation;
		float time;
	};

	struct ScaleKey
	{
		glm::vec3 scale;
		float time;
	};

	struct Animation
	{
		std::string name;
		std::vector<std::vector<TranslationKey>> translations;
		std::vector<std::vector<RotationKey>> rotations;
		std::vector<std::vector<ScaleKey>> scales;
		float duration;
	};

	typedef std::vector<Animation> Animations;
}

#endif // _CE_ANIMATION_H_