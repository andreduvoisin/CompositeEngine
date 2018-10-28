#ifndef _CE_TOGGLE_PAUSE_MESSAGE_H_
#define _CE_TOGGLE_PAUSE_MESSAGE_H_

#include "UIMessage.h"
#include "JsonDeserializer.h"

class TogglePauseRequest : public UIMessageRequest
{
public:
	TogglePauseRequest();

protected:
	void DeserializeInternal(const JsonDeserializer& deserializer) override;
};

class TogglePauseResponse : public UIMessageResponse
{
public:
	TogglePauseResponse(bool isPaused);

protected:
	void SerializeInternal(JsonSerializer& serializer) const override;

private:
	bool isPaused;
};

#endif // _CE_TOGGLE_PAUSE_MESSAGE_H_
