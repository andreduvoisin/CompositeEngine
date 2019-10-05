#include "UIAppBrowser.h"

UIAppBrowser::UIAppBrowser(const CefRefPtr<UIBrowserProcessHandler>& browserProcessHandler)
    : browserProcessHandler(browserProcessHandler)
{
}

CefRefPtr<CefBrowserProcessHandler> UIAppBrowser::GetBrowserProcessHandler()
{
    return browserProcessHandler;
}
