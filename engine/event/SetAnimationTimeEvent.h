#ifndef _CE_SET_ANIMATION_TIME_EVENT_H_
#define _CE_SET_ANIMATION_TIME_EVENT_H_

#include "core/Event.h"

struct SetAnimationTimeEvent : Event
{
	SetAnimationTimeEvent();
	SetAnimationTimeEvent* Clone() const override;

	float time;
};

#endif // _CE_SET_ANIMATION_TIME_EVENT_H_
