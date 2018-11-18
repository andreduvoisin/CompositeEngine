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

void EventSystem::EnqueueEvent(const Event& event)
{
	eventQueue.push(event.Clone());
}

void EventSystem::DispatchEvents()
{
	while (!eventQueue.empty())
	{
		Event* event = eventQueue.front();

		auto it = registeredListeners.find(event->type);
		if (it == registeredListeners.end())
		{
			// TODO: Unique pointer instead of duplicated code.
			eventQueue.pop();
			delete event;

			continue;
		}

		const std::vector<EventListener*>& listeners = it->second;
		for (size_t i = 0; i < listeners.size(); ++i)
		{
			listeners[i]->OnEvent(*event);
		}

		// TODO: Unique pointer instead of duplicated code.
		eventQueue.pop();
		delete event;
	}
}
