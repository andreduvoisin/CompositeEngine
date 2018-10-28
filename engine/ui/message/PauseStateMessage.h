#ifndef _CE_PAUSE_STATE_MESSAGE_H_
#define _CE_PAUSE_STATE_MESSAGE_H_

#include "UIMessage.h"
#include "JsonDeserializer.h"

class PauseStateSubscription : public UIMessageRequest
{
public:
	PauseStateSubscription();

protected:
	void DeserializeInternal(const JsonDeserializer& deserializer) override;
};

class PauseStateStatus : public UIMessageResponse
{
public:
	PauseStateStatus(bool isPaused);

protected:
	void SerializeInternal(JsonSerializer& serializer) const override;

private:
	bool isPaused;
};

#endif // _CE_PAUSE_STATE_MESSAGE_H_
