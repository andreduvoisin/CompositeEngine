#include "UIApp.h"

UIApp::UIApp(CefRefPtr<UIBrowserProcessHandler> browserProcessHandler)
	: browserProcessHandler(browserProcessHandler)
{

}

CefRefPtr<CefBrowserProcessHandler> UIApp::GetBrowserProcessHandler()
{
	return browserProcessHandler;
}
