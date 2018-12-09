#include "ToggleBindPoseEvent.h"

#include "common/json/JsonSerializer.h"
#include "common/json/JsonDeserializer.h"

ToggleBindPoseEvent::ToggleBindPoseEvent()
	: Event(EventType::TOGGLE_BIND_POSE)
{

}

ToggleBindPoseEvent* ToggleBindPoseEvent::Clone() const
{
	return new ToggleBindPoseEvent(*this);
}
