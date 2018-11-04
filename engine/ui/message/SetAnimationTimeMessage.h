#ifndef _CE_SET_ANIMATION_TIME_MESSAGE_H_
#define _CE_SET_ANIMATION_TIME_MESSAGE_H_

#include "UIMessage.h"
#include "JsonDeserializer.h"

struct SetAnimationTimeRequest : UIMessageRequest
{
	SetAnimationTimeRequest();

	float time;

protected:
	void DeserializeInternal(const JsonDeserializer& deserializer) override;
};

#endif // _CE_SET_ANIMATION_TIME_MESSAGE_H_
