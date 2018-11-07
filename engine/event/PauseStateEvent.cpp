#include "PauseStateEvent.h"

#include "ui/message/JsonSerializer.h"
#include "ui/message/JsonDeserializer.h"

PauseStateEvent::PauseStateEvent()
	: Event(EventType::PAUSE_STATE)
{

}

PauseStateEvent* PauseStateEvent::Clone() const
{
	return new PauseStateEvent(*this);
}

void PauseStateEvent::SerializeInternal(JsonSerializer& serializer) const
{
	serializer.WriteBool("paused", paused);
}

void PauseStateEvent::DeserializeInternal(const JsonDeserializer& deserializer)
{
	paused = deserializer.GetBool("paused");
}

RequestPauseStateEvent::RequestPauseStateEvent()
	: Event(EventType::REQUEST_PAUSE_STATE)
{

}

RequestPauseStateEvent* RequestPauseStateEvent::Clone() const
{
	return new RequestPauseStateEvent(*this);
}
