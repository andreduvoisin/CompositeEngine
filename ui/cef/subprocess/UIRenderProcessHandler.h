#ifndef _CE_UI_RENDER_PROCESS_HANDLER_H_
#define _CE_UI_RENDER_PROCESS_HANDLER_H_

#include "include/cef_render_process_handler.h"
#include "include/wrapper/cef_message_router.h"

class UIRenderProcessHandler : public CefRenderProcessHandler
{
public:
	UIRenderProcessHandler(CefRefPtr<CefMessageRouterRendererSide> messageRouterRendererSide);

	// CefRenderProcessHandler Interface
	void OnContextCreated(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) override;
	void OnContextReleased(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) override;
	bool OnProcessMessageReceived(
		CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) override;

private:
	CefRefPtr<CefMessageRouterRendererSide> messageRouterRendererSide;

	// IMPLEMENT_* macros set access modifiers, so they must come last.
	IMPLEMENT_REFCOUNTING(UIRenderProcessHandler);
};

#endif //_CE_UI_RENDER_PROCESS_HANDLER_H_
