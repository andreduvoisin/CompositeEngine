#ifndef _CE_EVENT_SYSTEM_H_
#define _CE_EVENT_SYSTEM_H_

#include "Event.h"
#include "EventListener.h"
#include "ScheduledEvent.h"

#include <unordered_map>
#include <queue>

class EventSystem
{
public:
	void RegisterListener(EventListener* listener, EventType type);

	void SendEvent(const Event& event, EventListener& listener);
	void DispatchEvent(const Event& event);
	void EnqueueEvent(const Event& event);
	void EnqueueEventScheduled(const Event& event, uint64_t deliveryTicks);

	void DispatchEvents(uint64_t currentTicks);

private:
	void DispatchImmediateEvents();
	void DispatchScheduledEvents(uint64_t currentTicks);

	std::queue<Event*> immediateEventQueue;
	std::priority_queue<ScheduledEvent, std::vector<ScheduledEvent>, std::greater<>> scheduledEventQueue;
	std::unordered_map<EventType, std::vector<EventListener*>> registeredListeners;
};

#endif // _CE_EVENT_SYSTEM_H_
