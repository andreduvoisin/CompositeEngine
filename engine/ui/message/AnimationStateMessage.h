#ifndef _CE_ANIMATION_STATE_MESSAGE_H_
#define _CE_ANIMATION_STATE_MESSAGE_H_

#include "JsonSerializer.h"
#include "JsonDeserializer.h"

struct AnimationStateRequest
{
	static AnimationStateRequest Deserialize(const JsonDeserializer& deserializer)
	{
		return AnimationStateRequest();
	}
};

struct AnimationStateResponse
{
	float currentTime;
	float duration;

	static std::string Serialize(const AnimationStateResponse& message)
	{
		JsonSerializer serializer;
		serializer.WriteFloat("currentTime", message.currentTime);
		serializer.WriteFloat("duration", message.duration);
		return serializer.ToString();
	}
};

#endif // _CE_ANIMATION_STATE_MESSAGE_H_
