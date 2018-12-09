#include "UIApp.h"

UIApp::UIApp(
		CefRefPtr<UIBrowserProcessHandler> browserProcessHandler,
		CefRefPtr<UIRenderProcessHandler> renderProcessHandler)
	: browserProcessHandler(browserProcessHandler)
	, renderProcessHandler(renderProcessHandler)
{

}

CefRefPtr<CefBrowserProcessHandler> UIApp::GetBrowserProcessHandler()
{
	return browserProcessHandler;
}

CefRefPtr<CefRenderProcessHandler> UIApp::GetRenderProcessHandler()
{
	return renderProcessHandler;
}
