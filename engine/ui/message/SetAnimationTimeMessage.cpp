#include "SetAnimationTimeMessage.h"

SetAnimationTimeRequest::SetAnimationTimeRequest()
	: UIMessageRequest(UIMessageId::REQUEST_SET_ANIMATION_TIME)
{

}

void SetAnimationTimeRequest::DeserializeInternal(const JsonDeserializer& deserializer)
{
	time = deserializer.GetFloat("time");
}
