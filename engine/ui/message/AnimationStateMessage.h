#ifndef _CE_ANIMATION_STATE_MESSAGE_H_
#define _CE_ANIMATION_STATE_MESSAGE_H_

#include "UIMessage.h"
#include "JsonDeserializer.h"

struct AnimationStateSubscription : UIMessageRequest
{
	AnimationStateSubscription();

protected:
	void DeserializeInternal(const JsonDeserializer& deserializer) override;
};

struct AnimationStateStatus : UIMessageResponse
{
	AnimationStateStatus();

	float currentTime;
	float duration;

protected:
	void SerializeInternal(JsonSerializer& serializer) const override;
};

#endif // _CE_ANIMATION_STATE_MESSAGE_H_
