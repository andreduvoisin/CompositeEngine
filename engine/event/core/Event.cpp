#include "Event.h"

#include "common/json/JsonSerializer.h"

Event::Event(EventType type)
	: type(type)
{

}

std::string Event::Serialize() const
{
	JsonSerializer serializer;
	serializer.WriteUint32("type", static_cast<uint32_t>(type));
	SerializeInternal(serializer);
	return serializer.ToString();
}

void Event::Deserialize(const JsonDeserializer& deserializer)
{
	DeserializeInternal(deserializer);
}

void Event::SerializeInternal(JsonSerializer& serializer) const
{

}

void Event::DeserializeInternal(const JsonDeserializer& deserializer)
{

}
