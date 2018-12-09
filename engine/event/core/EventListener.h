#ifndef _CE_EVENT_LISTENER_H_
#define _CE_EVENT_LISTENER_H_

#include "Event.h"

class EventListener
{
public:
	virtual ~EventListener() = default;
	virtual void OnEvent(const Event& event) = 0;
};

#endif // _CE_EVENT_LISTENER_H_
