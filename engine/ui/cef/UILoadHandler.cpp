#include "UILoadHandler.h"

void UILoadHandler::OnLoadingStateChange(
		CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward)
{
	if (!isLoading)
	{
		printf("FINISHED LOADING!\n");
		// TODO: Send initialized message.
	}
}
