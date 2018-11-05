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
	printf("GetViewRect: %u, %u, %u, %u\n", rect.x, rect.y, rect.width, rect.height);
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
	printf("PaintElementType: %u, w*h*4: %u \t\t ", type, width*height*4);
	for (const CefRect& rect : dirtyRects)
	{
		printf("rect: %u, %u, %u, %u \t\t", rect.x, rect.y, rect.width, rect.height);
	}
	printf("\n");
	memcpy(this->buffer, buffer, width * height * 4);
}
