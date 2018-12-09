#include "SetRenderModeEvent.h"

#include "common/json/JsonSerializer.h"
#include "common/json/JsonDeserializer.h"

SetRenderModeEvent::SetRenderModeEvent()
	: Event(EventType::SET_RENDER_MODE)
{

}

SetRenderModeEvent* SetRenderModeEvent::Clone() const
{
	return new SetRenderModeEvent(*this);
}

void SetRenderModeEvent::SerializeInternal(JsonSerializer& serializer) const
{
	serializer.WriteUint32("mode", mode);
}

void SetRenderModeEvent::DeserializeInternal(const JsonDeserializer& deserializer)
{
	mode = deserializer.GetUint32("mode");
}