#ifndef _CE_TOGGLE_PAUSE_MESSAGE_H_
#define _CE_TOGGLE_PAUSE_MESSAGE_H_

#include "InputBufferStream.h"
#include "OutputBufferStream.h"

struct TogglePauseRequest
{
	static std::string Serialize(const TogglePauseRequest& message, OutputBufferStream& stream)
	{
		stream.Write(message);
		return stream.ToString();
	}

	static TogglePauseRequest Deserialize(InputBufferStream& stream)
	{
		return stream.Read<TogglePauseRequest>();
	}
};

struct TogglePauseResponse
{
	bool isPaused;

	static std::string Serialize(const TogglePauseResponse& message, OutputBufferStream& stream)
	{
		stream.Write(message);
		return stream.ToString();
	}

	static TogglePauseResponse Deserialize(InputBufferStream& stream)
	{
		return stream.Read<TogglePauseResponse>();
	}
};

#endif // _CE_TOGGLE_PAUSE_MESSAGE_H_
