#ifndef _CE_UI_QUERY_RESPONDER_H_
#define _CE_UI_QUERY_RESPONDER_H_

#include "UIQuery.h"
#include "event/core/EventReceiver.h"
#include "message/UIMessage.h"

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
	void InitializeQueriesForEvent(EventId eventId);

	void RegisterQueryForEvent(EventId eventId, const UIQuery& query);
	void BroadcastMessageForEvent(EventId eventId, const UIMessageResponse& message);

	void HandleTogglePauseEvent(const Event& event);
	void HandlePauseStateEvent(const Event& event);
	void HandleSetAnimationTimeEvent(const Event& event);
	void HandleAnimationStateEvent(const Event& event);

	std::unordered_map<EventId, std::list<UIQuery>> eventToQueries;
};

#endif // _CE_UI_QUERY_RESPONDER_H_
