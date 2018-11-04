#include "PauseStateMessage.h"

PauseStateSubscription::PauseStateSubscription()
	: UIMessageRequest(UIMessageId::SUBSCRIPTION_PAUSE_STATE)
{

}

void PauseStateSubscription::DeserializeInternal(const JsonDeserializer& deserializer)
{

}

PauseStateStatus::PauseStateStatus()
	: UIMessageResponse(UIMessageId::STATUS_PAUSE_STATE)
{

}

void PauseStateStatus::SerializeInternal(JsonSerializer& serializer) const
{
	serializer.WriteBool("paused", paused);
}
