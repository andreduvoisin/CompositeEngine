#include "UIAppRender.h"

UIAppRender::UIAppRender(CefRefPtr<UIRenderProcessHandler> renderProcessHandler)
	: renderProcessHandler(renderProcessHandler)
{

}

CefRefPtr<CefRenderProcessHandler> UIAppRender::GetRenderProcessHandler()
{
	return renderProcessHandler;
}
