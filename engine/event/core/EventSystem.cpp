#include "EventSystem.h"

void EventSystem::RegisterListener(EventListener* listener, EventType type)
{
	auto it = registeredListeners.find(type);

	if (it == registeredListeners.end())
	{
		it = registeredListeners.insert({ type, std::vector<EventListener*>() }).first;
	}

	it->second.push_back(listener);
}

void EventSystem::SendEvent(const Event& event, EventListener& listener)
{
	listener.OnEvent(event);
}

void EventSystem::DispatchEvent(const Event& event)
{
	auto it = registeredListeners.find(event.type);
	if (it == registeredListeners.end())
	{
		return;
	}

	const std::vector<EventListener*>& listeners = it->second;
	for (size_t i = 0; i < listeners.size(); ++i)
	{
		listeners[i]->OnEvent(event);
	}
}

void EventSystem::EnqueueEvent(const Event& event)
{
	immediateEventQueue.push(event.Clone());
}

void EventSystem::EnqueueEventScheduled(const Event& event, uint64_t deliveryTicks)
{
	ScheduledEventKey key;
	key.deliveryTicks = deliveryTicks;
	key.type = event.type;
	scheduledEventQueue[key] = event.Clone();
}

void EventSystem::DispatchEvents(uint64_t currentTicks)
{
	DispatchImmediateEvents();
	DispatchScheduledEvents(currentTicks);
}

void EventSystem::DispatchImmediateEvents()
{
	while (!immediateEventQueue.empty())
	{
		Event* event = immediateEventQueue.front();

		DispatchEvent(*event);

		delete event;
		immediateEventQueue.pop();
	}
}

void EventSystem::DispatchScheduledEvents(uint64_t currentTicks)
{
	auto it = scheduledEventQueue.begin();

	while (it != scheduledEventQueue.end())
	{
		if (it->first.deliveryTicks > currentTicks)
		{
			break;
		}

		DispatchEvent(*it->second);

		delete it->second;
		it = scheduledEventQueue.erase(it);
	}
}
