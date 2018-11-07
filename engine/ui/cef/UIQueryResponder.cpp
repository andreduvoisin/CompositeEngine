#include "UIQueryResponder.h"

#include "event/core/EventSystem.h"

UIQueryResponder::UIQueryResponder(EventSystem* eventSystem)
{
	EventType types [] = {
		EventType::PAUSE_STATE,
		EventType::ANIMATION_STATE
	};

	for (EventType type : types)
	{
		eventSystem->RegisterReceiverForEvent(this, type);
		eventToQueries[type] = std::list<UIQuery>();
	}
}

void UIQueryResponder::OnEvent(const Event& event)
{
	BroadcastEvent(event);
}

void UIQueryResponder::AddQuery(EventType type, const UIQuery& query)
{
	switch (type)
	{
		case EventType::REQUEST_PAUSE_STATE:
		{
			RegisterQueryForEvent(EventType::PAUSE_STATE, query);
			break;
		}

		case EventType::REQUEST_ANIMATION_STATE:
		{
			RegisterQueryForEvent(EventType::ANIMATION_STATE, query);
			break;
		}

		case EventType::TOGGLE_PAUSE:
		case EventType::SET_ANIMATION_TIME:
		case EventType::TOGGLE_RENDER_MODE:
		{
			SendSuccessResponse(query);
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

void UIQueryResponder::SendSuccessResponse(const UIQuery& query)
{
	query.callback->Success("0");
}
