#include "AnimationStateEvent.h"

AnimationStateEvent::AnimationStateEvent()
	: Event(EventId::ANIMATION_STATE)
{

}

AnimationStateEvent* AnimationStateEvent::Clone() const
{
	return new AnimationStateEvent(*this);
}

RequestAnimationStateEvent::RequestAnimationStateEvent()
	: Event(EventId::REQUEST_ANIMATION_STATE)
{

}

RequestAnimationStateEvent* RequestAnimationStateEvent::Clone() const
{
	return new RequestAnimationStateEvent(*this);
}
