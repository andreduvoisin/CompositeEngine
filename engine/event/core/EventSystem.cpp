#include "EventSystem.h"

void EventSystem::RegisterReceiverForEvent(EventReceiver* receiver, EventId eventId)
{
	auto it = registeredReceivers.find(eventId);

	if (it == registeredReceivers.end())
	{
		it = registeredReceivers.insert({ eventId, std::vector<EventReceiver*>() }).first;
	}

	it->second.push_back(receiver);
}

void EventSystem::SendEvent(const Event& event, EventReceiver& receiver)
{
	receiver.OnEvent(event);
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

		auto it = registeredReceivers.find(event->id);
		if (it == registeredReceivers.end())
		{
			continue;
		}

		const std::vector<EventReceiver*>& eventReceivers = it->second;
		for (size_t i = 0; i < eventReceivers.size(); ++i)
		{
			eventReceivers[i]->OnEvent(*event);
		}

		eventQueue.pop();
		delete event;
	}
}
