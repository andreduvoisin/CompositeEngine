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
	serializer.WriteUint32("fps", fps);
}

void FpsStateEvent::DeserializeInternal(const JsonDeserializer& deserializer)
{
	fps = deserializer.GetUint32("fps");
}

RequestFpsStateEvent::RequestFpsStateEvent()
	: Event(EventType::REQUEST_FPS_STATE)
{

}

RequestFpsStateEvent* RequestFpsStateEvent::Clone() const
{
	return new RequestFpsStateEvent(*this);
}
