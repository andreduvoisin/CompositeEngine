#include "ToggleRenderModeMessage.h"

ToggleRenderModeRequest::ToggleRenderModeRequest()
	: UIMessageRequest(UIMessageId::REQUEST_TOGGLE_RENDER_MODE)
{

}

void ToggleRenderModeRequest::DeserializeInternal(const JsonDeserializer& deserializer)
{

}
