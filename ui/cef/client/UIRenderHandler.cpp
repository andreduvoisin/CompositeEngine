#include "UIRenderHandler.h"

UIRenderHandler::UIRenderHandler(unsigned width, unsigned height)
	: width(width)
	, height(height)
	, viewBuffer(new char[width * height * 4])
	, popupBuffer(nullptr)
	, popupRect(0, 0, 0, 0)
{

}

void UIRenderHandler::Resize(unsigned width, unsigned height)
{
	this->width = width;
	this->height = height;

	delete[] viewBuffer;
	viewBuffer = new char[width * height * 4];
}

void UIRenderHandler::Render()
{
	// TODO: Move code here from Main.cpp
}

void UIRenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
	rect = CefRect(0, 0, width, height);
}

void UIRenderHandler::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
{
	if (!show)
	{
		popupRect.Set(0, 0, 0, 0);
		delete[] popupBuffer;
		popupBuffer = nullptr;
	}
}

void UIRenderHandler::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
{
	popupRect = rect;
	popupBuffer = new char[popupRect.width * popupRect.height * 4];
}

void UIRenderHandler::OnPaint(
		CefRefPtr<CefBrowser> browser,
		PaintElementType type,
		const RectList& dirtyRects,
		const void* buffer,
		int width,
		int height)
{
	switch (type)
	{
		case PET_VIEW:
		{
			memcpy(this->viewBuffer, buffer, width * height * 4);
			break;
		}

		case PET_POPUP:
		{
			memcpy(this->popupBuffer, buffer, width * height * 4);
			break;
		}
	}
}
