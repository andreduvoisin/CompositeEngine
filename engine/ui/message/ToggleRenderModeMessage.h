#ifndef _CE_TOGGLE_RENDER_MODE_REQUEST_H_
#define _CE_TOGGLE_RENDER_MODE_REQUEST_H_

#include "UIMessage.h"
#include "JsonDeserializer.h"

struct ToggleRenderModeRequest : UIMessageRequest
{
	ToggleRenderModeRequest();

protected:
	void DeserializeInternal(const JsonDeserializer& deserializer) override;
};

#endif // _CE_TOGGLE_RENDER_MODE_REQUEST_H_
