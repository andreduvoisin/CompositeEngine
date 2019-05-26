#include "AnimationStateEvent.h"

#include "common/json/JsonSerializer.h"
#include "common/json/JsonDeserializer.h"

AnimationStateEvent::AnimationStateEvent()
	: Event(EventType::ANIMATION_STATE)
{

}

AnimationStateEvent* AnimationStateEvent::Clone() const
{
	return new AnimationStateEvent(*this);
}

void AnimationStateEvent::SerializeInternal(JsonSerializer& serializer) const
{
	serializer.WriteFloat("currentTime", currentTime);
	serializer.WriteFloat("duration", duration);
	serializer.WriteString("availableAnimationName1", availableAnimationName1);
	serializer.WriteFloat("availableAnimationDuration1", availableAnimationDuration1);
	serializer.WriteString("availableAnimationName2", availableAnimationName2);
	serializer.WriteFloat("availableAnimationDuration2", availableAnimationDuration2);
}

void AnimationStateEvent::DeserializeInternal(const JsonDeserializer& deserializer)
{
	currentTime = deserializer.GetFloat("currentTime");
	duration = deserializer.GetFloat("duration");
	availableAnimationName1 = deserializer.GetString("availableAnimationName1");
	availableAnimationDuration1 = deserializer.GetFloat("availableAnimationDuration1");
	availableAnimationName2 = deserializer.GetString("availableAnimationName2");
	availableAnimationDuration2 = deserializer.GetFloat("availableAnimationDuration2");
}

RequestAnimationStateEvent::RequestAnimationStateEvent()
	: Event(EventType::REQUEST_ANIMATION_STATE)
{

}

RequestAnimationStateEvent* RequestAnimationStateEvent::Clone() const
{
	return new RequestAnimationStateEvent(*this);
}
