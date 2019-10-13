#ifndef _CE_UI_RENDER_HANDLER_H_
#define _CE_UI_RENDER_HANDLER_H_

#include "include/cef_render_handler.h"
#include <gsl/gsl>

class UIRenderHandler : public CefRenderHandler
{
public:
    UIRenderHandler(uint32_t width, uint32_t height);

    void Resize(uint32_t width, uint32_t height);
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
    const std::byte* GetViewBuffer() const
    {
        return viewBuffer.data();
    }
    const std::byte* GetPopupBuffer() const
    {
        return popupBuffer.data();
    }
    const CefRect& GetPopupRect() const
    {
        return popupRect;
    }
    bool HasPopup() const
    {
        return !popupRect.IsEmpty();
    }

private:
    uint32_t width, height;

    // From the CefRenderHandler OnPaint documentation:
    // |buffer| will be |width|*|height|*4 bytes in size and
    // represents a BGRA image with an upper-left origin.
    std::vector<std::byte> viewBuffer;
    std::vector<std::byte> popupBuffer;

    CefRect popupRect;

    // IMPLEMENT_* macros set access modifiers, so they must come last.
    IMPLEMENT_REFCOUNTING(UIRenderHandler);
};

#endif //_CE_UI_RENDER_HANDLER_H_
