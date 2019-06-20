#ifndef _CE_ANIMATION_OPTIMIZER_H_
#define _CE_ANIMATION_OPTIMIZER_H_

#include <vector>

namespace CE
{
    struct Animation;
    using Animations = std::vector<Animation>;

    class AnimationOptimizer
    {
    public:
        explicit AnimationOptimizer(Animations* animations);
        AnimationOptimizer(
                Animations* animations,
                float translationTolerance,
                float rotationTolerance,
                float scaleTolerance,
                float hierarchicalTolerance);

        void OptimizeAnimations();

    private:
        void OptimizeAnimation(Animation& animation);

        Animations* animations;

        const float TRANSLATION_TOLERANCE;
        const float ROTATION_TOLERANCE;
        const float SCALE_TOLERANCE;
        const float HIERARCHICAL_TOLERANCE;
    };
}

#endif // _CE_ANIMATION_OPTIMIZER_H_