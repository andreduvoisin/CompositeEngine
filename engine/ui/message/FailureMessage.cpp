#include "FailureMessage.h"

FailureResponse::FailureResponse()
	: UIMessageResponse(UIMessageId::RESPONSE_FAILURE)
{

}

void FailureResponse::SerializeInternal(JsonSerializer& serializer) const
{

}
