#include "UIMessage.h"

UIMessage::UIMessage(UIMessageId id)
	: id(id)
{

}

UIMessageRequest::UIMessageRequest(UIMessageId id)
	: UIMessage(id)
{

}

void UIMessageRequest::Deserialize(const JsonDeserializer& deserializer)
{
	DeserializeInternal(deserializer);
}

UIMessageResponse::UIMessageResponse(UIMessageId id)
	: UIMessage(id)
{

}

std::string UIMessageResponse::Serialize() const
{
	JsonSerializer serializer;
	serializer.WriteUint32("id", GetId());
	SerializeInternal(serializer);
	return serializer.ToString();
}
