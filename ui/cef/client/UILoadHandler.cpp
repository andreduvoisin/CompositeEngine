#include "UILoadHandler.h"

static const uint32_t RELOAD_TIMER_INTERVAL_MS = 1000;

void UILoadHandler::OnLoadingStateChange(
		CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward)
{
	if (!isLoading)
	{
		printf("FINISHED LOADING!\n");

		if (!browser->HasDocument())
		{
			printf("RETRYING LOAD...\n");
			reloadTimerId = SDL_AddTimer(RELOAD_TIMER_INTERVAL_MS, &ReloadBrowserCallback, browser.get());
		}
		else
		{
			printf("LOAD SUCCESS!");
		}

		// TODO: Send initialized message.
	}
}

uint32_t UILoadHandler::ReloadBrowserCallback(uint32_t intervalMillis, void* param)
{
	CefBrowser* browser = (CefBrowser*) param;
	browser->Reload();
	return 0;
}
