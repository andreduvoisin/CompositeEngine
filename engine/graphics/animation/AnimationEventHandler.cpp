#include "AnimationEventHandler.h"

#include "Animation.h"
#include "AnimationComponent.h"

#include "event/core/Event.h"
#include "event/SetAnimationTimeEvent.h"
#include "core/clock/RealTimeClock.h"

namespace CE
{
	AnimationEventHandler::AnimationEventHandler(
			EventSystem* eventSystem,
			AnimationComponent* animationComponent)
		: eventSystem(eventSystem)
		, animationComponent(animationComponent)
		, previousAnimationTimeEventTicks(0)
	{
		eventSystem->RegisterListener(this, EventType::REQUEST_ANIMATION_STATE);
		eventSystem->RegisterListener(this, EventType::SET_ANIMATION_TIME);
	}

	void AnimationEventHandler::OnEvent(const Event& event)
	{
		switch (event.type)
		{
			case EventType::REQUEST_ANIMATION_STATE:
			{
				SendAnimationStateEvent();
				break;
			}

			case EventType::SET_ANIMATION_TIME:
			{
				HandleSetAnimationTimeEvent(event);
				break;
			}
		}
	}

	void AnimationEventHandler::SendAnimationStateEvent()
	{
		// TODO: Write event throttling class. see fpscounter
		uint64_t throttleTicks = CE::RealTimeClock::Get().GetNextTicksForInterval(1.f / 30.f);

		if (previousAnimationTimeEventTicks == throttleTicks)
		{
			return;
		}

		previousAnimationTimeEventTicks = throttleTicks;

		Animation* animation = &animationComponent->m_animations->at(animationComponent->m_currentAnimation);
		AnimationCache* animationCache = &animationComponent->m_animationCaches[animationComponent->m_currentAnimation];

		AnimationStateEvent animationStateEvent;
		animationStateEvent.currentTime = animationCache->currTime;
		animationStateEvent.duration = animation->duration;

		eventSystem->EnqueueEventScheduled(animationStateEvent, throttleTicks);
	}

	void AnimationEventHandler::HandleSetAnimationTimeEvent(const Event& event)
	{
		const SetAnimationTimeEvent& setAnimationTimeEvent = reinterpret_cast<const SetAnimationTimeEvent&>(event);

		// TODO: There's definitely more to do here. I think this should also do a FindInterpolationKeys?
		AnimationCache* animationCache = &animationComponent->m_animationCaches[animationComponent->m_currentAnimation];
		animationCache->currTime = setAnimationTimeEvent.time;

		SendAnimationStateEvent();
	}
}
