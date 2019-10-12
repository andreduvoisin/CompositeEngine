#include "UILoadHandler.h"

#include <iostream>

void UILoadHandler::OnLoadingStateChange(
        CefRefPtr<CefBrowser> /*browser*/,
        bool isLoading,
        bool /*canGoBack*/,
        bool /*canGoForward*/)
{
    if (!isLoading)
    {
        std::cout << "FINISHED LOADING!\n";
        // TODO: Send initialized message.
    }
}
