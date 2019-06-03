#ifndef _CE_WINDOWS_MESSAGE_EVENT_H_
#define _CE_WINDOWS_MESSAGE_EVENT_H_

#include "core/Event.h"

namespace CE
{
    struct WindowsMessageEvent : Event
    {
        WindowsMessageEvent();
        WindowsMessageEvent* Clone() const override;

		void* userdata;
		void* hWnd;
		uint32_t message;
		uint64_t wParam;
		int64_t lParam;
    };
}

#endif // _CE_WINDOWS_MESSAGE_EVENT_H_