#ifndef _CE_PAUSE_STATE_EVENT_H_
#define _CE_PAUSE_STATE_EVENT_H_

#include "core/Event.h"

struct PauseStateEvent : Event
{
	PauseStateEvent();
	PauseStateEvent* Clone() const override;

	bool paused;
};

struct RequestPauseStateEvent : Event
{
	RequestPauseStateEvent();
	RequestPauseStateEvent* Clone() const override;
};

#endif // _CE_PAUSE_STATE_EVENT_H_
