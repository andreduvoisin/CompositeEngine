#include "SetAnimationTimeEvent.h"

#include "ui/message/JsonSerializer.h"
#include "ui/message/JsonDeserializer.h"

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
