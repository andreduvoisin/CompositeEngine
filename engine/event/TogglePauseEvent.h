#ifndef _CE_TOGGLE_PAUSE_EVENT_H_
#define _CE_TOGGLE_PAUSE_EVENT_H_

#include "core/Event.h"

struct TogglePauseEvent : Event
{
	TogglePauseEvent();
	TogglePauseEvent* Clone() const override;
};

#endif // _CE_TOGGLE_PAUSE_EVENT_H_
