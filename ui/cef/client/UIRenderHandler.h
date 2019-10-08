#ifndef _CE_UI_RENDER_HANDLER_H_
#define _CE_UI_RENDER_HANDLER_H_

#include "include/cef_render_handler.h"

class UIRenderHandler : public CefRenderHandler
{
public:
    UIRenderHandler(unsigned width, unsigned height);

    void Resize(unsigned width, unsigned height);
    void Render();

    // CefRenderHandler Interface
    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
    void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) override;
    void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) override;
    void OnPaint(
            CefRefPtr<CefBrowser> browser,
            PaintElementType type,
            const RectList& dirtyRects,
            const void* buffer,
            int width,
            int height) override;

    // TODO: Remove and render with Render().
    char* GetViewBuffer() const
    {
        return viewBuffer;
    }
    char* GetPopupBuffer() const
    {
        return popupBuffer;
    }
    const CefRect& GetPopupRect() const
    {
        return popupRect;
    }

private:
    unsigned width, height;

    // From the CefRenderHandler OnPaint documentation:
    // |buffer| will be |width|*|height|*4 bytes in size and
    // represents a BGRA image with an upper-left origin.
    char* viewBuffer;
    char* popupBuffer;

    CefRect popupRect;

    // IMPLEMENT_* macros set access modifiers, so they must come last.
    IMPLEMENT_REFCOUNTING(UIRenderHandler);
};

#endif //_CE_UI_RENDER_HANDLER_H_
