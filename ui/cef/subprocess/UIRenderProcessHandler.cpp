#include "UIRenderProcessHandler.h"

UIRenderProcessHandler::UIRenderProcessHandler(CefRefPtr<CefMessageRouterRendererSide> messageRouterRendererSide)
	: messageRouterRendererSide(messageRouterRendererSide)
{
	
}

void UIRenderProcessHandler::OnContextCreated(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context)
{
	messageRouterRendererSide->OnContextCreated(browser, frame, context);
}

void UIRenderProcessHandler::OnContextReleased(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context)
{
	messageRouterRendererSide->OnContextReleased(browser, frame, context);
}

bool UIRenderProcessHandler::OnProcessMessageReceived(
		CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message)
{
	bool handled = false;
	handled = messageRouterRendererSide->OnProcessMessageReceived(browser, source_process, message) || handled;
	return handled;
}
