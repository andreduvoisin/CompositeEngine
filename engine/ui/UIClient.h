#ifndef _CE_UI_CLIENT_H_
#define _CE_UI_CLIENT_H_

#include "UIRenderHandler.h"
#include "UILifeSpanHandler.h"
#include "UIRequestHandler.h"

#include "include/cef_client.h"
#include "include/wrapper/cef_message_router.h"

class UIClient : public CefClient
{
public:
	UIClient(
		CefRefPtr<UIRenderHandler> renderHandler,
		CefRefPtr<UILifeSpanHandler> lifeSpanHandler,
		CefRefPtr<UIRequestHandler> requestHandler,
		CefRefPtr<CefMessageRouterBrowserSide> messageRouterBrowserSide);

	// CefClient Interface
	CefRefPtr<CefRenderHandler> GetRenderHandler() override;
	CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
	CefRefPtr<CefRequestHandler> GetRequestHandler() override;
	bool OnProcessMessageReceived(
		CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) override;

private:
	CefRefPtr<UIRenderHandler> renderHandler;
	CefRefPtr<UILifeSpanHandler> lifeSpanHandler;
	CefRefPtr<UIRequestHandler> requestHandler;
	CefRefPtr<CefMessageRouterBrowserSide> messageRouterBrowserSide;

	// IMPLEMENT_* macros set access modifiers, so they must come last.
	IMPLEMENT_REFCOUNTING(UIClient);
};

#endif //_CE_UI_CLIENT_H_