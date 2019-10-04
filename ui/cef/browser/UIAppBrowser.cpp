#include "UIAppBrowser.h"

UIAppBrowser::UIAppBrowser(CefRefPtr<UIBrowserProcessHandler> browserProcessHandler)
    : browserProcessHandler(browserProcessHandler)
{
}

CefRefPtr<CefBrowserProcessHandler> UIAppBrowser::GetBrowserProcessHandler()
{
    return browserProcessHandler;
}
