#ifndef _CE_EVENT_ID_H_
#define _CE_EVENT_ID_H_

#include <cstdint>

enum class EventId : uint32_t
{
	SET_ANIMATION_TIME = 0,
	REQUEST_ANIMATION_STATE,
	ANIMATION_STATE,
	TOGGLE_PAUSE,
	REQUEST_PAUSE_STATE,
	PAUSE_STATE
};

#endif // _CE_EVENT_ID_H_
