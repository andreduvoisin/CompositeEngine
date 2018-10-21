#include "UILifeSpanHandler.h"

UILifeSpanHandler::UILifeSpanHandler(CefRefPtr<CefMessageRouterBrowserSide> messageRouterBrowserSide)
	: messageRouterBrowserSide(messageRouterBrowserSide)
{

}

void UILifeSpanHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	messageRouterBrowserSide->OnBeforeClose(browser);
}
