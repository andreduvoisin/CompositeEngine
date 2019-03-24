#ifndef _CE_UI_APP_RENDERER_H_
#define _CE_UI_APP_RENDERER_H_

#include "UIRenderProcessHandler.h"

#include "include/cef_app.h"

class UIAppRenderer : public CefApp
{
public:
	UIAppRenderer(CefRefPtr<UIRenderProcessHandler> renderProcessHandler);

	// CefApp Interface
	CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

private:
	CefRefPtr<UIRenderProcessHandler> renderProcessHandler;

	// IMPLEMENT_* macros set access modifiers, so they must come last.
	IMPLEMENT_REFCOUNTING(UIAppRenderer);
	DISALLOW_COPY_AND_ASSIGN(UIAppRenderer);
};

#endif //_CE_UI_APP_RENDERER_H_
