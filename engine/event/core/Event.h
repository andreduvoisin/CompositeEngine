#ifndef _CE_EVENT_H_
#define _CE_EVENT_H_

#include "EventType.h"

#include <string>

class JsonSerializer;
class JsonDeserializer;

struct Event
{
	Event(EventType type);
	virtual ~Event() = default;

	std::string Serialize() const;
	void Deserialize(const JsonDeserializer& deserializer);

	virtual Event* Clone() const = 0;

	EventType type;

protected:
	virtual void SerializeInternal(JsonSerializer& serializer) const;
	virtual void DeserializeInternal(const JsonDeserializer& deserializer);
};

#endif // _CE_EVENT_H_
