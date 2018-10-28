#ifndef _CE_UI_MESSAGE_H_
#define _CE_UI_MESSAGE_H_

#include "JsonSerializer.h"
#include "JsonDeserializer.h"

#include "UIMessageId.h"

#include <string>

class UIMessage
{
public:
	UIMessage(UIMessageId id);
	virtual ~UIMessage() = default;

	UIMessageId GetId() const { return id; } 

private:
	UIMessageId id;
};

class UIMessageRequest : public UIMessage
{
public:
	UIMessageRequest(UIMessageId id);
	void Deserialize(const JsonDeserializer& deserializer);

protected:
	virtual void DeserializeInternal(const JsonDeserializer& deserializer) = 0;
};

class UIMessageResponse : public UIMessage
{
public:
	UIMessageResponse(UIMessageId id);
	std::string Serialize() const;

protected:
	virtual void SerializeInternal(JsonSerializer& serializer) const = 0;
};

#endif // _CE_UI_MESSAGE_H_
