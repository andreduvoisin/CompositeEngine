#include "UIQueryResponder.h"

#include "event/AnimationStateEvent.h"
#include "event/PauseStateEvent.h"
#include "event/core/EventSystem.h"

#include "message/AnimationStateMessage.h"
#include "message/PauseStateMessage.h"
#include "message/SuccessMessage.h"

UIQueryResponder::UIQueryResponder(EventSystem* eventSystem)
{
	EventId eventIds [] = {
		EventId::TOGGLE_PAUSE,
		EventId::PAUSE_STATE,
		EventId::SET_ANIMATION_TIME,
		EventId::ANIMATION_STATE
	};

	for (EventId eventId : eventIds)
	{
		eventSystem->RegisterReceiverForEvent(this, eventId);
		InitializeQueriesForEvent(eventId);
	}
}

void UIQueryResponder::OnEvent(const Event& event)
{
	switch (event.id)
	{
		case EventId::TOGGLE_PAUSE:
		{
			HandleTogglePauseEvent(event);
			break;
		}

		case EventId::PAUSE_STATE:
		{
			HandlePauseStateEvent(event);
			break;
		}

		case EventId::SET_ANIMATION_TIME:
		{
			HandleSetAnimationTimeEvent(event);
			break;
		}

		case EventId::ANIMATION_STATE:
		{
			HandleAnimationStateEvent(event);
			break;
		}
	}
}

void UIQueryResponder::AddQuery(const UIQuery& query)
{
	switch (query.messageId)
	{
		case UIMessageId::REQUEST_TOGGLE_PAUSE:
		{
			RegisterQueryForEvent(EventId::TOGGLE_PAUSE, query);
			break;
		}

		case UIMessageId::SUBSCRIPTION_PAUSE_STATE:
		{
			RegisterQueryForEvent(EventId::PAUSE_STATE, query);
			break;
		}

		case UIMessageId::REQUEST_SET_ANIMATION_TIME:
		{
			RegisterQueryForEvent(EventId::SET_ANIMATION_TIME, query);
			break;
		}

		case UIMessageId::SUBSCRIPTION_ANIMATION_STATE:
		{
			RegisterQueryForEvent(EventId::ANIMATION_STATE, query);
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

void UIQueryResponder::InitializeQueriesForEvent(EventId eventId)
{
	eventToQueries[eventId] = std::list<UIQuery>();
}

void UIQueryResponder::RegisterQueryForEvent(EventId eventId, const UIQuery& query)
{
	eventToQueries[eventId].push_back(query);
}

void UIQueryResponder::BroadcastMessageForEvent(EventId eventId, const UIMessageResponse& message)
{
	auto it = eventToQueries.find(eventId);
	if (it == eventToQueries.end())
	{
		return;
	}

	std::list<UIQuery>& queries = it->second;
	if (queries.empty())
	{
		return;
	}

	std::string buffer = message.Serialize();

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

void UIQueryResponder::HandleTogglePauseEvent(const Event& event)
{
	BroadcastMessageForEvent(event.id, SuccessResponse());
}

void UIQueryResponder::HandlePauseStateEvent(const Event& event)
{
	const PauseStateEvent& pauseStateEvent = reinterpret_cast<const PauseStateEvent&>(event);

	PauseStateStatus pauseStateStatus;
	pauseStateStatus.paused = pauseStateEvent.paused;

	BroadcastMessageForEvent(event.id, pauseStateStatus);
}

void UIQueryResponder::HandleSetAnimationTimeEvent(const Event& event)
{
	BroadcastMessageForEvent(event.id, SuccessResponse());
}

void UIQueryResponder::HandleAnimationStateEvent(const Event& event)
{
	const AnimationStateEvent& animationStateEvent = reinterpret_cast<const AnimationStateEvent&>(event);

	AnimationStateStatus animationStateStatus;
	animationStateStatus.currentTime = animationStateEvent.currentTime;
	animationStateStatus.duration = animationStateEvent.duration;

	BroadcastMessageForEvent(event.id, animationStateStatus);
}
