#ifndef _CE_FPS_STATE_EVENT_H_
#define _CE_FPS_STATE_EVENT_H_

#include "core/Event.h"

struct FpsStateEvent : Event
{
	FpsStateEvent();
	FpsStateEvent* Clone() const override;

	uint32_t fps;

protected:
	void SerializeInternal(JsonSerializer& serializer) const override;
	void DeserializeInternal(const JsonDeserializer& deserializer) override;
};

struct RequestFpsStateEvent : Event
{
	RequestFpsStateEvent();
	RequestFpsStateEvent* Clone() const override;
};

#endif // _CE_FPS_STATE_EVENT_H_
