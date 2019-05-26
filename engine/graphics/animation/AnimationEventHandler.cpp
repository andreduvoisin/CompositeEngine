#include "AnimationEventHandler.h"

#include "Animation.h"
#include "AnimationComponent.h"

#include "event/core/Event.h"
#include "event/SetAnimationTimeEvent.h"
#include "event/SetAnimationEvent.h"
#include "core/clock/RealTimeClock.h"

namespace CE
{
	AnimationEventHandler::AnimationEventHandler(
			EventSystem* eventSystem,
			AnimationComponent* animationComponent)
		: eventSystem(eventSystem)
		, animationComponent(animationComponent)
	{
		eventSystem->RegisterListener(this, EventType::REQUEST_ANIMATION_STATE);
		eventSystem->RegisterListener(this, EventType::SET_ANIMATION_TIME);
		eventSystem->RegisterListener(this, EventType::SET_ANIMATION);
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

			case EventType::SET_ANIMATION:
			{
				HandleSetAnimation(event);
				break;
			}
		}
	}

	void AnimationEventHandler::SendAnimationStateEvent()
	{
		uint64_t throttleTicks = RealTimeClock::Get().GetNextTicksForInterval(1.f / 30.f);

		Animation* animation = &animationComponent->m_animations->at(animationComponent->m_currentAnimation);
		AnimationCache* animationCache = &animationComponent->m_animationCaches[animationComponent->m_currentAnimation];

		AnimationStateEvent animationStateEvent;
		animationStateEvent.currentTime = animationCache->currTime;
		animationStateEvent.duration = animation->duration;
		animationStateEvent.availableAnimationName1 = animationComponent->m_animations->at(0).name;
		animationStateEvent.availableAnimationDuration1 = animationComponent->m_animations->at(0).duration;
		animationStateEvent.availableAnimationName2 = animationComponent->m_animations->at(1).name;
		animationStateEvent.availableAnimationDuration2 = animationComponent->m_animations->at(1).duration;

		eventSystem->EnqueueEventThrottled(animationStateEvent, throttleTicks);
	}

	void AnimationEventHandler::HandleSetAnimationTimeEvent(const Event& event)
	{
		const SetAnimationTimeEvent& setAnimationTimeEvent = reinterpret_cast<const SetAnimationTimeEvent&>(event);

		// TODO: There's definitely more to do here. I think this should also do a FindInterpolationKeys?
		AnimationCache* animationCache = &animationComponent->m_animationCaches[animationComponent->m_currentAnimation];
		animationCache->currTime = setAnimationTimeEvent.time;

		SendAnimationStateEvent();
	}

	void AnimationEventHandler::HandleSetAnimation(const Event& event)
	{
		const SetAnimationEvent& setAnimationEvent = reinterpret_cast<const SetAnimationEvent&>(event);

		for (int i = 0; i < animationComponent->m_animations->size(); ++i)
		{
			if (animationComponent->m_animations->at(i).name == setAnimationEvent.name)
			{
				AnimationCache* animationCache = &animationComponent->m_animationCaches[animationComponent->m_currentAnimation];
				animationCache->currTime = 0.f;

				animationComponent->m_currentAnimation = i;
				break;
			}
		}

		SendAnimationStateEvent();
	}
}
