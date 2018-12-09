#include "UIRequestHandler.h"

UIRequestHandler::UIRequestHandler(CefRefPtr<CefMessageRouterBrowserSide> messageRouterBrowserSide)
	: messageRouterBrowserSide(messageRouterBrowserSide)
{

}

void UIRequestHandler::OnRenderProcessTerminated(
		CefRefPtr<CefBrowser> browser,
		TerminationStatus status)
{
	messageRouterBrowserSide->OnRenderProcessTerminated(browser);
}

bool UIRequestHandler::OnBeforeBrowse(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		bool user_gesture,
		bool is_redirect)
{
	messageRouterBrowserSide->OnBeforeBrowse(browser, frame);
	return false;
}
