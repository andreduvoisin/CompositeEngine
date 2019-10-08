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
		bool userGesture,
		bool isRedirect)
{
	messageRouterBrowserSide->OnBeforeBrowse(browser, frame);
	return false;
}
