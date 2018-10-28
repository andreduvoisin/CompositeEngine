#ifndef _CE_FAILURE_MESSAGE_H_
#define _CE_FAILURE_MESSAGE_H_

#include "UIMessage.h"

class FailureResponse : public UIMessageResponse
{
public:
	FailureResponse();

protected:
	void SerializeInternal(JsonSerializer& serializer) const override;
};

#endif // _CE_FAILURE_MESSAGE_H_
