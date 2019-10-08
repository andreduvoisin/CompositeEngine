#ifndef _CE_UI_QUERY_RESPONDER_H_
#define _CE_UI_QUERY_RESPONDER_H_

#include "UIQuery.h"
#include "event/core/EventListener.h"
#include <unordered_map>

#include <list>

class EventSystem;

class UIQueryResponder : public EventListener
{
public:
    explicit UIQueryResponder(EventSystem* eventSystem);

    // EventListener Interface
    void OnEvent(const Event& event) override;

    void AddQuery(EventType type, const UIQuery& query);
    void RemoveQuery(int64_t queryId);

private:
    void RegisterQueryForEvent(EventType type, const UIQuery& query);
    void BroadcastEvent(const Event& event);

    std::unordered_map<EventType, std::list<UIQuery>> eventToQueries;
};

#endif // _CE_UI_QUERY_RESPONDER_H_
