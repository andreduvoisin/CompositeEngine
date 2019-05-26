#ifndef _CE_ANIMATION_STATE_EVENT_H_
#define _CE_ANIMATION_STATE_EVENT_H_

#include "core/Event.h"

struct AnimationStateEvent : Event
{
	AnimationStateEvent();
	AnimationStateEvent* Clone() const override;

	float currentTime;
	float duration;
	std::string availableAnimationName1;
	float availableAnimationDuration1;
	std::string availableAnimationName2;
	float availableAnimationDuration2;

protected:
	void SerializeInternal(JsonSerializer& serializer) const override;
	void DeserializeInternal(const JsonDeserializer& deserializer) override;
};

struct RequestAnimationStateEvent : Event
{
	RequestAnimationStateEvent();
	RequestAnimationStateEvent* Clone() const override;
};

#endif // _CE_ANIMATION_STATE_EVENT_H_
