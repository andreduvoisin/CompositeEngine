#ifndef _CE_UI_LOAD_HANDLER_H_
#define _CE_UI_LOAD_HANDLER_H_

#include "include/cef_load_handler.h"

class UILoadHandler : public CefLoadHandler
{
public:
    // CefLoadHandler Interface
    void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
            override;

    // IMPLEMENT_* macros set access modifiers, so they must come last.
    IMPLEMENT_REFCOUNTING(UILoadHandler);
};

#endif //_CE_UI_LOAD_HANDLER_H_
