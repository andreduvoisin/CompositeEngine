#include "SetAnimationTimeEvent.h"

#include "common/json/JsonSerializer.h"
#include "common/json/JsonDeserializer.h"

SetAnimationTimeEvent::SetAnimationTimeEvent()
	: Event(EventType::SET_ANIMATION_TIME)
{

}

SetAnimationTimeEvent* SetAnimationTimeEvent::Clone() const
{
	return new SetAnimationTimeEvent(*this);
}

void SetAnimationTimeEvent::SerializeInternal(JsonSerializer& serializer) const
{
	serializer.WriteFloat("time", time);
}

void SetAnimationTimeEvent::DeserializeInternal(const JsonDeserializer& deserializer)
{
	time = deserializer.GetFloat("time");
}
