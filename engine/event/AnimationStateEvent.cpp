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
}

void AnimationStateEvent::DeserializeInternal(const JsonDeserializer& deserializer)
{
	currentTime = deserializer.GetFloat("currentTime");
	duration = deserializer.GetFloat("duration");
}

RequestAnimationStateEvent::RequestAnimationStateEvent()
	: Event(EventType::REQUEST_ANIMATION_STATE)
{

}

RequestAnimationStateEvent* RequestAnimationStateEvent::Clone() const
{
	return new RequestAnimationStateEvent(*this);
}
