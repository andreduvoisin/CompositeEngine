#include "FpsStateEvent.h"

#include "common/json/JsonSerializer.h"
#include "common/json/JsonDeserializer.h"

FpsStateEvent::FpsStateEvent()
	: Event(EventType::FPS_STATE)
{

}

FpsStateEvent* FpsStateEvent::Clone() const
{
	return new FpsStateEvent(*this);
}

void FpsStateEvent::SerializeInternal(JsonSerializer& serializer) const
{
	serializer.WriteBool("fps", fps);
}

void FpsStateEvent::DeserializeInternal(const JsonDeserializer& deserializer)
{
	fps = deserializer.GetBool("fps");
}

RequestFpsStateEvent::RequestFpsStateEvent()
	: Event(EventType::REQUEST_FPS_STATE)
{

}

RequestFpsStateEvent* RequestFpsStateEvent::Clone() const
{
	return new RequestFpsStateEvent(*this);
}
