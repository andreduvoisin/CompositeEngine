#ifndef _CE_SUCCESS_MESSAGE_H_
#define _CE_SUCCESS_MESSAGE_H_

#include "UIMessage.h"

struct SuccessResponse : UIMessageResponse
{
	SuccessResponse();

protected:
	void SerializeInternal(JsonSerializer& serializer) const override;
};

#endif // _CE_SUCCESS_MESSAGE_H_
