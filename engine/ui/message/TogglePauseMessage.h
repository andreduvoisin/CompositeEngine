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

#endif // _CE_TOGGLE_PAUSE_MESSAGE_H_
