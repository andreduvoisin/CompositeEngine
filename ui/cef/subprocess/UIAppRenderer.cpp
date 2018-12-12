#include "UIAppRenderer.h"

UIAppRenderer::UIAppRenderer(CefRefPtr<UIRenderProcessHandler> renderProcessHandler)
	: renderProcessHandler(renderProcessHandler)
{

}

CefRefPtr<CefRenderProcessHandler> UIAppRenderer::GetRenderProcessHandler()
{
	return renderProcessHandler;
}
