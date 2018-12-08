#ifndef _CE_TOGGLE_BIND_POSE_EVENT_H_
#define _CE_TOGGLE_BIND_POSE_EVENT_H_

#include "core/Event.h"

struct ToggleBindPoseEvent : Event
{
	ToggleBindPoseEvent();
	ToggleBindPoseEvent* Clone() const override;

protected:
	void SerializeInternal(JsonSerializer& serializer) const override;
	void DeserializeInternal(const JsonDeserializer& deserializer) override;
};

#endif // _CE_TOGGLE_BIND_POSE_EVENT_H_
