#include "PauseStateMessage.h"

PauseStateSubscription::PauseStateSubscription()
	: UIMessageRequest(UIMessageId::SUBSCRIPTION_PAUSE_STATE)
{

}

void PauseStateSubscription::DeserializeInternal(const JsonDeserializer& deserializer)
{

}

PauseStateStatus::PauseStateStatus(bool isPaused)
	: UIMessageResponse(UIMessageId::STATUS_PAUSE_STATE)
	, isPaused(isPaused)
{

}

void PauseStateStatus::SerializeInternal(JsonSerializer& serializer) const
{
	serializer.WriteBool("isPaused", isPaused);
}
