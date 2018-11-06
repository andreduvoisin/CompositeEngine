#ifndef _CE_EVENT_SYSTEM_H_
#define _CE_EVENT_SYSTEM_H_

#include "Event.h"
#include "EventReceiver.h"

#include <unordered_map>
#include <queue>

class EventSystem
{
public:
	void RegisterReceiverForEvent(EventReceiver* receiver, EventType type);

	void SendEvent(const Event& event, EventReceiver& receiver);
	void EnqueueEvent(const Event& event);

	void DispatchEvents();

private:
	std::queue<Event*> eventQueue;
	std::unordered_map<EventType, std::vector<EventReceiver*>> registeredReceivers;
};

#endif // _CE_EVENT_SYSTEM_H_
