#ifndef _CE_UI_APP_H_
#define _CE_UI_APP_H_

#include "UIBrowserProcessHandler.h"

#include "include/cef_app.h"

class UIApp : public CefApp
{
public:
	UIApp(CefRefPtr<UIBrowserProcessHandler> browserProcessHandler);

	// CefApp Interface
	CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;

private:
	CefRefPtr<UIBrowserProcessHandler> browserProcessHandler;

	// IMPLEMENT_* macros set access modifiers, so they must come last.
	IMPLEMENT_REFCOUNTING(UIApp);
};

#endif //_CE_UI_APP_H_
