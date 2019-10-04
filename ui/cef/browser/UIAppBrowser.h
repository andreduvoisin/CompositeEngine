#ifndef _CE_UI_APP_BROWSER_H_
#define _CE_UI_APP_BROWSER_H_

#include "UIBrowserProcessHandler.h"
#include "include/cef_app.h"

class UIAppBrowser : public CefApp
{
public:
    explicit UIAppBrowser(CefRefPtr<UIBrowserProcessHandler> browserProcessHandler);
    ~UIAppBrowser() override = default;
    UIAppBrowser(UIAppBrowser&&) = delete;
    UIAppBrowser& operator=(UIAppBrowser&&) = delete;

    // CefApp Interface
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;

private:
    CefRefPtr<UIBrowserProcessHandler> browserProcessHandler;

    DISALLOW_COPY_AND_ASSIGN(UIAppBrowser);

    // IMPLEMENT_* macros set access modifiers, so they must come last.
    IMPLEMENT_REFCOUNTING(UIAppBrowser);
};

#endif //_CE_UI_APP_BROWSER_H_
