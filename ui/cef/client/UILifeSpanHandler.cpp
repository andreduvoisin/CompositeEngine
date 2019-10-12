#include "UILifeSpanHandler.h"

UILifeSpanHandler::UILifeSpanHandler(const CefRefPtr<CefMessageRouterBrowserSide>& messageRouterBrowserSide)
    : messageRouterBrowserSide(messageRouterBrowserSide)
{
}

void UILifeSpanHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    messageRouterBrowserSide->OnBeforeClose(browser);
}
