#ifndef _CE_UI_QUERY_HANDLER_H_
#define _CE_UI_QUERY_HANDLER_H_

#include "include/wrapper/cef_message_router.h"

class EventSystem;
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

	void OnQueryCanceled(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 queryId) override;

private:
	EventSystem* eventSystem;
	UIQueryResponder* queryResponder;
};

#endif //_CE_UI_QUERY_HANDLER_H_
