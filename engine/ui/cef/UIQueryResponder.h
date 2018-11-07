#ifndef _CE_UI_QUERY_RESPONDER_H_
#define _CE_UI_QUERY_RESPONDER_H_

#include "UIQuery.h"
#include "event/core/EventReceiver.h"

#include "include/wrapper/cef_message_router.h"

#include <unordered_map>

class EventSystem;

class UIQueryResponder : public EventReceiver
{
public:
	UIQueryResponder(EventSystem* eventSystem);

	// EventReceiver Interface
	void OnEvent(const Event& event) override;

	void AddQuery(const UIQuery& query);
	void RemoveQuery(int64_t queryId);

private:
	void InitializeQueriesForEvent(EventType type);

	void RegisterQueryForEvent(EventType type, const UIQuery& query);
	void BroadcastEvent(const Event& event);

	std::unordered_map<EventType, std::list<UIQuery>> eventToQueries;
};

#endif // _CE_UI_QUERY_RESPONDER_H_
