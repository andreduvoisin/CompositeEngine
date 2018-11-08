#include "UIRenderHandler.h"

UIRenderHandler::UIRenderHandler(unsigned width, unsigned height)
	: width(width)
	, height(height)
	, buffer(new char[width * height * 4])
	, popupBuffer(nullptr)
	, popupRect(0, 0, 0, 0)
{

}

void UIRenderHandler::Resize(unsigned width, unsigned height)
{
	this->width = width;
	this->height = height;

	delete[] buffer;
	buffer = new char[width * height * 4];
}

void UIRenderHandler::Render()
{
	// TODO: Move code here from Main.cpp
}

bool UIRenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
	//printf("GetViewRect: %u, %u, %u, %u\n", rect.x, rect.y, rect.width, rect.height);
	rect = CefRect(0, 0, width, height);
	return true;
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
	printf("OnPopupSize: %u, %u, %u, %u \n", rect.x, rect.y, rect.width, rect.height);
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
	printf("PaintElementType: %u, w*h*4: %u \t\t ", type, width*height*4);
	for (const CefRect& rect : dirtyRects)
	{
		printf("rect: %u, %u, %u, %u \t\t", rect.x, rect.y, rect.width, rect.height);
	}
	printf("\n");
	switch (type)
	{
		case PET_VIEW:
		{
			//printf("PET_VIEW\n");
			for (auto it = dirtyRects.begin(); it != dirtyRects.end(); ++it)
			{
				UpdateBuffer(reinterpret_cast<const char*>(buffer), it->x, it->y, it->width, it->height);
				//for (int i = 0; i < it->height; ++i)
				//{
				//	int offsetSrc = i * it->width * 4;
				//	int offsetDst = (((it->y + i) * it->width) + it->x) * 4;
				//	memcpy(this->buffer + offsetDst, (char*)buffer + offsetSrc, it->width * 4);
				//}
			}
			if (!popupRect.IsEmpty())
			{
				browser->GetHost()->Invalidate(PET_POPUP);
			}
			//memcpy(this->buffer, buffer, width * height * 4);
			break;
		}

		case PET_POPUP:
		{
			//printf("PET_POPUP\n");
			//UpdateBuffer(reinterpret_cast<const char*>(buffer), popupRect.x, popupRect.y, width, height);
			memcpy(this->popupBuffer, buffer, width * height * 4);
			//for (int i = 0; i < height; ++i)
			//{
			//	int offsetSrc = i * width * 4;
			//	int offsetDst = (((popupRect.y + i) * width) + popupRect.x) * 4;
			//	memcpy(this->buffer + offsetDst, (char*)buffer + offsetSrc, width * 4);
			//}
			break;
		}
	}
}

void UIRenderHandler::UpdateBuffer(const char* buffer, int x, int y, int width, int height)
{
	//printf("START\n");
	for (int i = 0; i < height; ++i)
	{
		int offsetSrc = i * width * 4;
		int offsetDst = (((y + i) * this->width) + x) * 4;
		//printf("offsetSrc: %i, offsetDst: %i\n", offsetSrc, offsetDst);
		memcpy(this->buffer + offsetDst, buffer + offsetSrc, width * 4);
	}
	//printf("FINISH\n");
}
