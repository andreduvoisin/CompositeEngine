#include "UIClient.h"

UIClient::UIClient(CefRefPtr<UIRenderHandler> renderHandler)
	: renderHandler(renderHandler)
{

}

CefRefPtr<CefRenderHandler> UIClient::GetRenderHandler()
{
	return renderHandler;
}
