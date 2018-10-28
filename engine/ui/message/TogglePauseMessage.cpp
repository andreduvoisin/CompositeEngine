#include "TogglePauseMessage.h"

TogglePauseRequest::TogglePauseRequest()
	: UIMessageRequest(UIMessageId::TOGGLE_PAUSE)
{

}

void TogglePauseRequest::DeserializeInternal(const JsonDeserializer& deserializer)
{

}

TogglePauseResponse::TogglePauseResponse(bool isPaused)
	: UIMessageResponse(UIMessageId::TOGGLE_PAUSE)
	, isPaused(isPaused)
{

}

void TogglePauseResponse::SerializeInternal(JsonSerializer& serializer) const
{
	serializer.WriteBool("isPaused", isPaused);
}
