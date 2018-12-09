#ifndef _CE_SET_RENDER_MODE_H_
#define _CE_SET_RENDER_MODE_H_

#include "core/Event.h"

struct SetRenderModeEvent : Event
{
	SetRenderModeEvent();
	SetRenderModeEvent* Clone() const override;

	int32_t mode;

protected:
	void SerializeInternal(JsonSerializer& serializer) const override;
	void DeserializeInternal(const JsonDeserializer& deserializer) override;
};

#endif // _CE_SET_RENDER_MODE_H_
