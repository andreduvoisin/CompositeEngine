#include "UIQueryResponder.h"

#include "event/AnimationStateEvent.h"
#include "event/PauseStateEvent.h"
#include "event/core/EventSystem.h"

UIQueryResponder::UIQueryResponder(EventSystem* eventSystem)
{
	EventType types [] = {
		//EventType::TOGGLE_PAUSE,
		EventType::PAUSE_STATE,
		//EventType::SET_ANIMATION_TIME,
		EventType::ANIMATION_STATE
	};

	for (EventType type : types)
	{
		eventSystem->RegisterReceiverForEvent(this, type);
		InitializeQueriesForEvent(type);
	}
}

void UIQueryResponder::OnEvent(const Event& event)
{
	switch (event.type)
	{
		case EventType::PAUSE_STATE:
		{
			BroadcastEvent(event);
			break;
		}

		case EventType::ANIMATION_STATE:
		{
			BroadcastEvent(event);
			break;
		}
	}
}

void UIQueryResponder::AddQuery(const UIQuery& query)
{
	switch (query.eventType)
	{
		case EventType::TOGGLE_PAUSE:
		{
			query.callback->Success("0");
			break;
		}

		case EventType::REQUEST_PAUSE_STATE:
		{
			RegisterQueryForEvent(EventType::PAUSE_STATE, query);
			break;
		}

		case EventType::SET_ANIMATION_TIME:
		{
			query.callback->Success("0");
			break;
		}

		case EventType::REQUEST_ANIMATION_STATE:
		{
			RegisterQueryForEvent(EventType::ANIMATION_STATE, query);
			break;
		}

		case EventType::TOGGLE_RENDER_MODE:
		{
			query.callback->Success("0");
			break;
		}
	}
}

void UIQueryResponder::RemoveQuery(int64_t queryId)
{
	for (auto& eventToQueryIt : eventToQueries)
	{
		std::list<UIQuery>& queries = eventToQueryIt.second;

		for (auto queriesIt = queries.begin(); queriesIt != queries.end(); ++queriesIt)
		{
			if (queriesIt->queryId == queryId)
			{
				queries.erase(queriesIt);
				break;
			}
		}
	}
}

void UIQueryResponder::InitializeQueriesForEvent(EventType type)
{
	eventToQueries[type] = std::list<UIQuery>();
}

void UIQueryResponder::RegisterQueryForEvent(EventType type, const UIQuery& query)
{
	eventToQueries[type].push_back(query);
}

void UIQueryResponder::BroadcastEvent(const Event& event)
{
	auto it = eventToQueries.find(event.type);
	if (it == eventToQueries.end())
	{
		return;
	}

	std::list<UIQuery>& queries = it->second;
	if (queries.empty())
	{
		return;
	}

	std::string buffer = event.Serialize();

	auto queriesIt = queries.begin();
	while (queriesIt != queries.end())
	{
		queriesIt->callback->Success(buffer);

		if (queriesIt->persistent)
		{
			++queriesIt;
		}
		else
		{
			queriesIt = queries.erase(queriesIt);
		}
	}
}
