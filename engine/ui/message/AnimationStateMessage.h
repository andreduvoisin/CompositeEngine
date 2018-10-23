#ifndef _CE_ANIMATION_STATE_MESSAGE_H_
#define _CE_ANIMATION_STATE_MESSAGE_H_

#include "InputBufferStream.h"
#include "OutputBufferStream.h"

struct AnimationStateRequest
{
	static std::string Serialize(const AnimationStateRequest& message, OutputBufferStream& stream)
	{
		stream.Write(message);
		return stream.ToString();
	}

	static AnimationStateRequest Deserialize(InputBufferStream& stream)
	{
		return stream.Read<AnimationStateRequest>();
	}
};

struct AnimationStateResponse
{
	unsigned currentTime;
	unsigned duration;

	static std::string Serialize(const AnimationStateResponse& message, OutputBufferStream& stream)
	{
		stream.Write(message);
		return stream.ToString();
	}

	static AnimationStateResponse Deserialize(InputBufferStream& stream)
	{
		return stream.Read<AnimationStateResponse>();
	}
};

#endif // _CE_ANIMATION_STATE_MESSAGE_H_
