#ifndef _CE_PAUSE_STATE_MESSAGE_H_
#define _CE_PAUSE_STATE_MESSAGE_H_

#include "UIMessage.h"
#include "JsonDeserializer.h"

struct PauseStateSubscription : UIMessageRequest
{
	PauseStateSubscription();

protected:
	void DeserializeInternal(const JsonDeserializer& deserializer) override;
};

struct PauseStateStatus : UIMessageResponse
{
	PauseStateStatus();

	bool paused;

protected:
	void SerializeInternal(JsonSerializer& serializer) const override;
};

#endif // _CE_PAUSE_STATE_MESSAGE_H_
