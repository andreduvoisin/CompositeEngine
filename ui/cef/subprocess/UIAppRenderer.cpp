#include "UIAppRenderer.h"

UIAppRenderer::UIAppRenderer(const CefRefPtr<UIRenderProcessHandler>& renderProcessHandler)
    : renderProcessHandler(renderProcessHandler)
{
}

CefRefPtr<CefRenderProcessHandler> UIAppRenderer::GetRenderProcessHandler()
{
    return renderProcessHandler;
}
