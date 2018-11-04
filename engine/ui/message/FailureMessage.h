#ifndef _CE_FAILURE_MESSAGE_H_
#define _CE_FAILURE_MESSAGE_H_

#include "UIMessage.h"

struct FailureResponse : UIMessageResponse
{
	FailureResponse();

protected:
	void SerializeInternal(JsonSerializer& serializer) const override;
};

#endif // _CE_FAILURE_MESSAGE_H_
