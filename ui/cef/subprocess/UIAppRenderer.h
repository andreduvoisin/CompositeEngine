#ifndef _CE_UI_APP_RENDERER_H_
#define _CE_UI_APP_RENDERER_H_

#include "UIRenderProcessHandler.h"
#include "include/cef_app.h"

class UIAppRenderer : public CefApp
{
public:
    explicit UIAppRenderer(const CefRefPtr<UIRenderProcessHandler>& renderProcessHandler);
    ~UIAppRenderer() override = default;

    UIAppRenderer(const UIAppRenderer&) = delete;
    UIAppRenderer(UIAppRenderer&&) = delete;
    UIAppRenderer& operator=(const UIAppRenderer&) = delete;
    UIAppRenderer& operator=(UIAppRenderer&&) = delete;

    // CefApp Interface
    CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

private:
    CefRefPtr<UIRenderProcessHandler> renderProcessHandler;

    // IMPLEMENT_* macros set access modifiers, so they must come last.
    IMPLEMENT_REFCOUNTING(UIAppRenderer);
};

#endif //_CE_UI_APP_RENDERER_H_
