#include "UIRenderHandler.h"

UIRenderHandler::UIRenderHandler(uint32_t width, uint32_t height)
    : width(width)
    , height(height)
    , viewBuffer(width * height * 4)
    , popupRect(0, 0, 0, 0)
{
}

void UIRenderHandler::Resize(uint32_t width, uint32_t height)
{
    this->width = width;
    this->height = height;

    viewBuffer.resize(width * height * 4);
    viewBuffer.shrink_to_fit();
}

void UIRenderHandler::Render()
{
    // TODO: Move code here from Main.cpp
}

void UIRenderHandler::GetViewRect(CefRefPtr<CefBrowser> /*browser*/, CefRect& rect)
{
    rect = CefRect(0, 0, width, height);
}

void UIRenderHandler::OnPopupShow(CefRefPtr<CefBrowser> /*browser*/, bool show)
{
    if (!show)
    {
        popupRect.Set(0, 0, 0, 0);

        popupBuffer.resize(0);
        popupBuffer.shrink_to_fit();
    }
}

void UIRenderHandler::OnPopupSize(CefRefPtr<CefBrowser> /*browser*/, const CefRect& rect)
{
    popupRect = rect;

    popupBuffer.resize(popupRect.width * popupRect.height * 4);
    popupBuffer.shrink_to_fit();
}

void UIRenderHandler::OnPaint(
        CefRefPtr<CefBrowser> /*browser*/,
        PaintElementType type,
        const RectList& /*dirtyRects*/,
        const void* buffer,
        int width,
        int height)
{
    switch (type)
    {
        case PET_VIEW:
        {
            memcpy(this->viewBuffer.data(), buffer, width * height * 4);
            break;
        }

        case PET_POPUP:
        {
            memcpy(this->popupBuffer.data(), buffer, width * height * 4);
            break;
        }
    }
}
