#ifndef _CE_UI_QUERY_HANDLER_H_
#define _CE_UI_QUERY_HANDLER_H_

#include "event/core/EventSystem.h"

#include "include/wrapper/cef_message_router.h"

class JsonDeserializer;
class UIQueryResponder;

class UIQueryHandler : public CefMessageRouterBrowserSide::Handler
{
public:
	UIQueryHandler(EventSystem* eventSystem, UIQueryResponder* queryResponder);

	bool OnQuery(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 queryId,
		const CefString& request,
		bool persistent,
		CefRefPtr<Callback> callback) override;

	void OnQueryCanceled(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 queryId) override;

private:
	template<typename T>
	void SendEvent(const JsonDeserializer& deserializer);

	void SendSuccessResponse(CefRefPtr<Callback> callback);
	void AddQueryToResponder(
		EventType type,
		int64 queryId,
		bool persistent,
		CefRefPtr<Callback> callback);

	EventSystem* eventSystem;
	UIQueryResponder* queryResponder;
};

template <typename T>
void UIQueryHandler::SendEvent(const JsonDeserializer& deserializer)
{
	T event;
	event.Deserialize(deserializer);
	eventSystem->EnqueueEvent(event);
}

#endif //_CE_UI_QUERY_HANDLER_H_
