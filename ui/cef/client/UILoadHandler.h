#ifndef _CE_UI_LOAD_HANDLER_H_
#define _CE_UI_LOAD_HANDLER_H_

#include "include/cef_load_handler.h"

#include <SDL_timer.h>

class UILoadHandler : public CefLoadHandler
{
public:
	// CefLoadHandler Interface
	void OnLoadingStateChange(
		CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward) override;

private:
	static uint32_t ReloadBrowserCallback(uint32_t intervalMillis, void* param);

	SDL_TimerID reloadTimerId;

	// IMPLEMENT_* macros set access modifiers, so they must come last.
	IMPLEMENT_REFCOUNTING(UILoadHandler);
};

#endif //_CE_UI_LOAD_HANDLER_H_
