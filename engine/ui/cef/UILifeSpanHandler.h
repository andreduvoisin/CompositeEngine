#ifndef _CE_UI_LIFE_SPAN_HANDLER_H_
#define _CE_UI_LIFE_SPAN_HANDLER_H_

#include "include/cef_life_span_handler.h"
#include "include/wrapper/cef_message_router.h"

class UILifeSpanHandler : public CefLifeSpanHandler
{
public:
	UILifeSpanHandler(CefRefPtr<CefMessageRouterBrowserSide> messageRouterBrowserSide);

	// CefLifeSpanHandler Interface
	void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

private:
	CefRefPtr<CefMessageRouterBrowserSide> messageRouterBrowserSide;

	// IMPLEMENT_* macros set access modifiers, so they must come last.
	IMPLEMENT_REFCOUNTING(UILifeSpanHandler);
};

#endif //_CE_UI_LIFE_SPAN_HANDLER_H_
