#ifndef _CE_UI_CONTEXT_MENU_HANDLER_H_
#define _CE_UI_CONTEXT_MENU_HANDLER_H_

#include "include/cef_context_menu_handler.h"

class UIContextMenuHandler : public CefContextMenuHandler
{
public:
	// CefContextMenuHandler Interface
	void OnBeforeContextMenu(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		CefRefPtr<CefMenuModel> model) override;

private:
	// IMPLEMENT_* macros set access modifiers, so they must come last.
	IMPLEMENT_REFCOUNTING(UIContextMenuHandler);
};

#endif //_CE_UI_CONTEXT_MENU_HANDLER_H_
