#include "TogglePauseMessage.h"

TogglePauseRequest::TogglePauseRequest()
	: UIMessageRequest(UIMessageId::REQUEST_TOGGLE_PAUSE)
{

}

void TogglePauseRequest::DeserializeInternal(const JsonDeserializer& deserializer)
{

}
