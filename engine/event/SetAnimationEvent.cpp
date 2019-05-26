#include "SetAnimationEvent.h"

#include "common/json/JsonSerializer.h"
#include "common/json/JsonDeserializer.h"

SetAnimationEvent::SetAnimationEvent()
	: Event(EventType::SET_ANIMATION)
{

}

SetAnimationEvent* SetAnimationEvent::Clone() const
{
	return new SetAnimationEvent(*this);
}

void SetAnimationEvent::SerializeInternal(JsonSerializer& serializer) const
{
	serializer.WriteString("name", name);
}

void SetAnimationEvent::DeserializeInternal(const JsonDeserializer& deserializer)
{
	name = deserializer.GetString("name");
}
