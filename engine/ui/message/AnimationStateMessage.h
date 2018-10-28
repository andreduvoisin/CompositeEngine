#ifndef _CE_ANIMATION_STATE_MESSAGE_H_
#define _CE_ANIMATION_STATE_MESSAGE_H_

#include "UIMessage.h"
#include "JsonDeserializer.h"

class AnimationStateSubscription : public UIMessageRequest
{
public:
	AnimationStateSubscription();

protected:
	void DeserializeInternal(const JsonDeserializer& deserializer) override;
};

class AnimationStateStatus : public UIMessageResponse
{
public:
	AnimationStateStatus(float currentTime, float duration);

protected:
	void SerializeInternal(JsonSerializer& serializer) const override;

private:
	float currentTime;
	float duration;
};

#endif // _CE_ANIMATION_STATE_MESSAGE_H_
