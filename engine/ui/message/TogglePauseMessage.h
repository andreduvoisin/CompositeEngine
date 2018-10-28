#ifndef _CE_TOGGLE_PAUSE_MESSAGE_H_
#define _CE_TOGGLE_PAUSE_MESSAGE_H_

#include "JsonSerializer.h"
#include "JsonDeserializer.h"

struct TogglePauseRequest
{
	static TogglePauseRequest Deserialize(const JsonDeserializer& deserializer)
	{
		return TogglePauseRequest();
	}
};

struct TogglePauseResponse
{
	bool isPaused;

	static std::string Serialize(const TogglePauseResponse& message)
	{
		JsonSerializer serializer;
		serializer.WriteBool("isPaused", message.isPaused);
		return serializer.ToString();
	}
};

#endif // _CE_TOGGLE_PAUSE_MESSAGE_H_
