#include "UILoadHandler.h"

void UILoadHandler::OnLoadingStateChange(
		CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward)
{
	if (!isLoading)
	{
		// TODO: Send initialized message.
	}
}
