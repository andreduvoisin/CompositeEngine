#ifndef _CE_UI_APP_RENDER_H_
#define _CE_UI_APP_RENDER_H_

#include "UIRenderProcessHandler.h"

#include "include/cef_app.h"

class UIAppRender : public CefApp
{
public:
	UIAppRender(CefRefPtr<UIRenderProcessHandler> renderProcessHandler);

	// CefApp Interface
	CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

private:
	CefRefPtr<UIRenderProcessHandler> renderProcessHandler;

	// IMPLEMENT_* macros set access modifiers, so they must come last.
	IMPLEMENT_REFCOUNTING(UIAppRender);
};

#endif //_CE_UI_APP_RENDER_H_
