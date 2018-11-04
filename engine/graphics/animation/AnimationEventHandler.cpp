#include "AnimationEventHandler.h"

#include "Animation.h"
#include "AnimationComponent.h"

#include "event/core/Event.h"
#include "event/SetAnimationTimeEvent.h"

namespace CE
{
	AnimationEventHandler::AnimationEventHandler(
			EventSystem* eventSystem,
			AnimationComponent* animationComponent)
		: eventSystem(eventSystem)
		, animationComponent(animationComponent)
	{
		eventSystem->RegisterReceiverForEvent(this, EventId::REQUEST_ANIMATION_STATE);
		eventSystem->RegisterReceiverForEvent(this, EventId::SET_ANIMATION_TIME);
	}

	void AnimationEventHandler::OnEvent(const Event& event)
	{
		switch (event.id)
		{
			case EventId::REQUEST_ANIMATION_STATE:
			{
				SendAnimationStateEvent();
				break;
			}

			case EventId::SET_ANIMATION_TIME:
			{
				HandleSetAnimationTimeEvent(event);
				break;
			}
		}
	}

	void AnimationEventHandler::SendAnimationStateEvent()
	{
		Animation* animation = &animationComponent->m_animations->at(animationComponent->m_currentAnimation);
		AnimationCache* animationCache = &animationComponent->m_animationCaches[animationComponent->m_currentAnimation];

		AnimationStateEvent animationStateEvent;
		animationStateEvent.currentTime = animationCache->currTime;
		animationStateEvent.duration = animation->duration;

		eventSystem->EnqueueEvent(animationStateEvent);
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
