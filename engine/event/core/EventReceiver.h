#ifndef _CE_EVENT_RECEIVER_H_
#define _CE_EVENT_RECEIVER_H_

#include "Event.h"

class EventReceiver
{
public:
	virtual ~EventReceiver() = default;
	virtual void OnEvent(const Event& event) = 0;
};

#endif // _CE_EVENT_RECEIVER_H_
