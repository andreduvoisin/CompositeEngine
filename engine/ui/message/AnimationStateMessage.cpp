#include "AnimationStateMessage.h"

AnimationStateSubscription::AnimationStateSubscription()
	: UIMessageRequest(UIMessageId::SUBSCRIPTION_ANIMATION_STATE)
{

}

void AnimationStateSubscription::DeserializeInternal(const JsonDeserializer& deserializer)
{

}

AnimationStateStatus::AnimationStateStatus()
	: UIMessageResponse(UIMessageId::STATUS_ANIMATION_STATE)
{

}

void AnimationStateStatus::SerializeInternal(JsonSerializer& serializer) const
{
	serializer.WriteFloat("currentTime", currentTime);
	serializer.WriteFloat("duration", duration);
}
