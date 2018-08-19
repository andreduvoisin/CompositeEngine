#ifndef _CE_UI_CLIENT_H_
#define _CE_UI_CLIENT_H_

#include "UIRenderHandler.h"

#include "include/cef_client.h"

class UIClient : public CefClient
{
public:
	UIClient(CefRefPtr<UIRenderHandler> renderHandler);

	// CefClient Interface
	CefRefPtr<CefRenderHandler> GetRenderHandler() override;

private:
	CefRefPtr<UIRenderHandler> renderHandler;

	// IMPLEMENT_* macros set access modifiers, so they must come last.
	IMPLEMENT_REFCOUNTING(UIClient);
};

#endif //_CE_UI_CLIENT_H_