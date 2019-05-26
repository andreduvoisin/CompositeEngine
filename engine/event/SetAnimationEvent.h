#ifndef _CE_SET_ANIMATION_EVENT_H_
#define _CE_SET_ANIMATION_EVENT_H_

#include "core/Event.h"

struct SetAnimationEvent : Event
{
	SetAnimationEvent();
	SetAnimationEvent* Clone() const override;

	std::string name;

protected:
	void SerializeInternal(JsonSerializer& serializer) const override;
	void DeserializeInternal(const JsonDeserializer& deserializer) override;
};

#endif // _CE_SET_ANIMATION_EVENT_H_
