#ifndef _CE_UI_APP_BROWSER_H_
#define _CE_UI_APP_BROWSER_H_

#include "UIBrowserProcessHandler.h"

#include "include/cef_app.h"

class UIAppBrowser : public CefApp
{
public:
	UIAppBrowser(CefRefPtr<UIBrowserProcessHandler> browserProcessHandler);

	// CefApp Interface
	CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;

private:
	CefRefPtr<UIBrowserProcessHandler> browserProcessHandler;

	// IMPLEMENT_* macros set access modifiers, so they must come last.
	IMPLEMENT_REFCOUNTING(UIAppBrowser);
	DISALLOW_COPY_AND_ASSIGN(UIAppBrowser);
};

#endif //_CE_UI_APP_BROWSER_H_
