#include "SuccessMessage.h"

SuccessResponse::SuccessResponse()
	: UIMessageResponse(UIMessageId::RESPONSE_SUCCESS)
{

}

void SuccessResponse::SerializeInternal(JsonSerializer& serializer) const
{

}
