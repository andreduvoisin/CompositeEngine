#ifndef _CE_ANIMATION_STATE_EVENT_H_
#define _CE_ANIMATION_STATE_EVENT_H_

#include "core/Event.h"

struct AnimationStateEvent : Event
{
	AnimationStateEvent();
	AnimationStateEvent* Clone() const override;

	float currentTime;
	float duration;
};

struct RequestAnimationStateEvent : Event
{
	RequestAnimationStateEvent();
	RequestAnimationStateEvent* Clone() const override;
};

#endif // _CE_ANIMATION_STATE_EVENT_H_
