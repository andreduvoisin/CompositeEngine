#include "AvailableAnimationEvent.h"

#include "common/json/JsonSerializer.h"
#include "common/json/JsonDeserializer.h"

AvailableAnimationEvent::AvailableAnimationEvent()
	: Event(EventType::AVAILABLE_ANIMATION)
{

}

AvailableAnimationEvent* AvailableAnimationEvent::Clone() const
{
	return new AvailableAnimationEvent(*this);
}

void AvailableAnimationEvent::SerializeInternal(JsonSerializer& serializer) const
{
	serializer.WriteString("name", name);
	serializer.WriteFloat("duration", duration);
}

void AvailableAnimationEvent::DeserializeInternal(const JsonDeserializer& deserializer)
{
	name = deserializer.GetString("name");
	duration = deserializer.GetFloat("duration");
}

RequestAvailableAnimationEvent::RequestAvailableAnimationEvent()
	: Event(EventType::REQUEST_AVAILABLE_ANIMATION)
{

}

RequestAvailableAnimationEvent* RequestAvailableAnimationEvent::Clone() const
{
	return new RequestAvailableAnimationEvent(*this);
}
