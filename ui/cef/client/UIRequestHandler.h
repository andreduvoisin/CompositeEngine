#ifndef _CE_UI_REQUEST_HANDLER_H_
#define _CE_UI_REQUEST_HANDLER_H_

#include "include/cef_request_handler.h"
#include "include/wrapper/cef_message_router.h"

class UIRequestHandler : public CefRequestHandler
{
public:
	UIRequestHandler(CefRefPtr<CefMessageRouterBrowserSide> messageRouterBrowserSide);

	// CefRequestHandler Interface
	void OnRenderProcessTerminated(
		CefRefPtr<CefBrowser> browser,
		TerminationStatus status) override;
	bool OnBeforeBrowse(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		bool user_gesture,
		bool is_redirect) override;

private:
	CefRefPtr<CefMessageRouterBrowserSide> messageRouterBrowserSide;

	// IMPLEMENT_* macros set access modifiers, so they must come last.
	IMPLEMENT_REFCOUNTING(UIRequestHandler);
};

#endif //_CE_UI_REQUEST_HANDLER_H_
