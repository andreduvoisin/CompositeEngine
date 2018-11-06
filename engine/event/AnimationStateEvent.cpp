#include "AnimationStateEvent.h"

AnimationStateEvent::AnimationStateEvent()
	: Event(EventType::ANIMATION_STATE)
{

}

AnimationStateEvent* AnimationStateEvent::Clone() const
{
	return new AnimationStateEvent(*this);
}

RequestAnimationStateEvent::RequestAnimationStateEvent()
	: Event(EventType::REQUEST_ANIMATION_STATE)
{

}

RequestAnimationStateEvent* RequestAnimationStateEvent::Clone() const
{
	return new RequestAnimationStateEvent(*this);
}
