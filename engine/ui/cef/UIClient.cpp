#include "UIClient.h"

UIClient::UIClient(
		CefRefPtr<UIRenderHandler> renderHandler,
		CefRefPtr<UILifeSpanHandler> lifeSpanHandler,
		CefRefPtr<UILoadHandler> loadHandler,
		CefRefPtr<UIRequestHandler> requestHandler,
		CefRefPtr<CefMessageRouterBrowserSide> messageRouterBrowserSide)
	: renderHandler(renderHandler)
	, lifeSpanHandler(lifeSpanHandler)
	, loadHandler(loadHandler)
	, requestHandler(requestHandler)
	, messageRouterBrowserSide(messageRouterBrowserSide)
{

}

CefRefPtr<CefRenderHandler> UIClient::GetRenderHandler()
{
	return renderHandler;
}

CefRefPtr<CefLifeSpanHandler> UIClient::GetLifeSpanHandler()
{
	return lifeSpanHandler;
}

CefRefPtr<CefLoadHandler> UIClient::GetLoadHandler()
{
	return loadHandler;
}

CefRefPtr<CefRequestHandler> UIClient::GetRequestHandler()
{
	return requestHandler;
}

bool UIClient::OnProcessMessageReceived(
		CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message)
{
	bool handled = false;
	handled = messageRouterBrowserSide->OnProcessMessageReceived(browser, source_process, message) || handled;
	return handled;
}
