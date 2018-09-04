#include "UIRenderHandler.h"

UIRenderHandler::UIRenderHandler(unsigned width, unsigned height)
	: width(width)
	, height(height)
{
	buffer = new char[width * height * 4];
}

void UIRenderHandler::Resize(unsigned width, unsigned height)
{
	this->width = width;
	this->height = height;

	delete buffer;
	buffer = new char[width * height * 4];
}

void UIRenderHandler::Render()
{
	// TODO: Move code here from Main.cpp
}

bool UIRenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
	rect = CefRect(0, 0, width, height);
	return true;
}

void UIRenderHandler::OnPaint(
		CefRefPtr<CefBrowser> browser,
		PaintElementType type,
		const RectList& dirtyRects,
		const void* buffer,
		int width,
		int height)
{
	memcpy(this->buffer, buffer, width * height * 4);
}
