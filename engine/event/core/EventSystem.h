#ifndef _CE_EVENT_SYSTEM_H_
#define _CE_EVENT_SYSTEM_H_

#include "Event.h"
#include "EventListener.h"
#include "ScheduledEventKey.h"

#include <unordered_map>
#include <queue>
#include <map>

class EventSystem
{
public:
	void RegisterListener(EventListener* listener, EventType type);

	void SendEvent(const Event& event, EventListener& listener);
	void DispatchEvent(const Event& event);

	void EnqueueEvent(const Event& event);
	void EnqueueEventScheduled(const Event& event, uint64_t deliveryTicks);
	template<typename T, typename = std::enable_if_t<std::is_base_of_v<Event, T>>>
	void EnqueueEventThrottled(T& event, uint64_t deliveryTicks);

	void DispatchEvents(uint64_t currentTicks);

private:
	void DispatchImmediateEvents();
	void DispatchScheduledEvents(uint64_t currentTicks);

	std::queue<Event*> immediateEventQueue;
	std::map<ScheduledEventKey, Event*> scheduledEventQueue;
	std::unordered_map<EventType, std::vector<EventListener*>> registeredListeners;
};

template<typename T, typename>
void EventSystem::EnqueueEventThrottled(T& event, uint64_t deliveryTicks)
{
	ScheduledEventKey key;
	key.deliveryTicks = deliveryTicks;
	key.type = event.type;

	Event*& scheduledEvent = scheduledEventQueue[key];
	if (scheduledEvent == nullptr)
	{
		scheduledEvent = event.Clone();
	}
	else
	{
		*static_cast<T*>(scheduledEvent) = std::move(event);
	}
}

#endif // _CE_EVENT_SYSTEM_H_
