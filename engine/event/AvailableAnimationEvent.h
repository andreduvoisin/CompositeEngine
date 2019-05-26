#ifndef _CE_AVAILABLE_ANIMATION_EVENT_H_
#define _CE_AVAILABLE_ANIMATION_EVENT_H_

#include "core/Event.h"

struct AvailableAnimationEvent : Event
{
	AvailableAnimationEvent();
	AvailableAnimationEvent* Clone() const override;

	std::string name;
	float duration;

protected:
	void SerializeInternal(JsonSerializer& serializer) const override;
	void DeserializeInternal(const JsonDeserializer& deserializer) override;
};

struct RequestAvailableAnimationEvent : Event
{
	RequestAvailableAnimationEvent();
	RequestAvailableAnimationEvent* Clone() const override;
};

#endif // _CE_AVAILABLE_ANIMATION_EVENT_H_
