#include "UIQueryResponder.h"

#include "event/AnimationStateEvent.h"
#include "event/PauseStateEvent.h"
#include "event/core/EventSystem.h"

#include "ui/message/AnimationStateMessage.h"
#include "ui/message/PauseStateMessage.h"
#include "ui/message/SuccessMessage.h"

UIQueryResponder::UIQueryResponder(EventSystem* eventSystem)
{
	EventType types [] = {
		EventType::TOGGLE_PAUSE,
		EventType::PAUSE_STATE,
		EventType::SET_ANIMATION_TIME,
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
		case EventType::TOGGLE_PAUSE:
		{
			HandleTogglePauseEvent(event);
			break;
		}

		case EventType::PAUSE_STATE:
		{
			HandlePauseStateEvent(event);
			break;
		}

		case EventType::SET_ANIMATION_TIME:
		{
			HandleSetAnimationTimeEvent(event);
			break;
		}

		case EventType::ANIMATION_STATE:
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
			RegisterQueryForEvent(EventType::TOGGLE_PAUSE, query);
			break;
		}

		case UIMessageId::SUBSCRIPTION_PAUSE_STATE:
		{
			RegisterQueryForEvent(EventType::PAUSE_STATE, query);
			break;
		}

		case UIMessageId::REQUEST_SET_ANIMATION_TIME:
		{
			RegisterQueryForEvent(EventType::SET_ANIMATION_TIME, query);
			break;
		}

		case UIMessageId::SUBSCRIPTION_ANIMATION_STATE:
		{
			RegisterQueryForEvent(EventType::ANIMATION_STATE, query);
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

void UIQueryResponder::BroadcastMessageForEvent(EventType type, const UIMessageResponse& message)
{
	auto it = eventToQueries.find(type);
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
	BroadcastMessageForEvent(event.type, SuccessResponse());
}

void UIQueryResponder::HandlePauseStateEvent(const Event& event)
{
	const PauseStateEvent& pauseStateEvent = reinterpret_cast<const PauseStateEvent&>(event);

	PauseStateStatus pauseStateStatus;
	pauseStateStatus.paused = pauseStateEvent.paused;

	BroadcastMessageForEvent(event.type, pauseStateStatus);
}

void UIQueryResponder::HandleSetAnimationTimeEvent(const Event& event)
{
	BroadcastMessageForEvent(event.type, SuccessResponse());
}

void UIQueryResponder::HandleAnimationStateEvent(const Event& event)
{
	const AnimationStateEvent& animationStateEvent = reinterpret_cast<const AnimationStateEvent&>(event);

	AnimationStateStatus animationStateStatus;
	animationStateStatus.currentTime = animationStateEvent.currentTime;
	animationStateStatus.duration = animationStateEvent.duration;

	BroadcastMessageForEvent(event.type, animationStateStatus);
}
