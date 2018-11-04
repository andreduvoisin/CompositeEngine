#ifndef _CE_EVENT_H_
#define _CE_EVENT_H_

#include "EventId.h"

struct Event
{
	Event(EventId id);
	virtual ~Event() = default;

	virtual Event* Clone() const = 0;

	EventId id;
};

#endif // _CE_EVENT_H_
