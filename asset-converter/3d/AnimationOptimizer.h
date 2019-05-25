#ifndef _CE_ANIMATION_OPTIMIZER_H_
#define _CE_ANIMATION_OPTIMIZER_H_

#include <vector>

namespace CE
{
	struct Animation;
	typedef std::vector<Animation> Animations;

	class AnimationOptimizer
	{
	public:
		AnimationOptimizer(Animations* animations);

		void OptimizeAnimations();

	private:
		void OptimizeAnimation(Animation& animation);

	private:
		Animations* animations;
	};
}

#endif // _CE_ANIMATION_OPTIMIZER_H_