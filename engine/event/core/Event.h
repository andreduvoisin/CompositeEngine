#ifndef _CE_EVENT_H_
#define _CE_EVENT_H_

#include "EventType.h"

struct Event
{
	Event(EventType type);
	virtual ~Event() = default;

	virtual Event* Clone() const = 0;

	EventType type;
};

#endif // _CE_EVENT_H_
