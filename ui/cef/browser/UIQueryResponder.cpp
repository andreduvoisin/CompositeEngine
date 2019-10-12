#include "UIQueryResponder.h"

#include "event/core/EventSystem.h"

#include <array>

UIQueryResponder::UIQueryResponder(EventSystem* eventSystem)
{
    std::array<EventType, 3> types = {EventType::PAUSE_STATE, EventType::ANIMATION_STATE, EventType::FPS_STATE};

    for (EventType type : types)
    {
        eventSystem->RegisterListener(this, type);
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

        case EventType::REQUEST_FPS_STATE:
        {
            RegisterQueryForEvent(EventType::FPS_STATE, query);
            break;
        }

        default:
            break;
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
