#include "WindowsMessageEvent.h"

namespace CE
{
	WindowsMessageEvent::WindowsMessageEvent()
		: Event(EventType::WINDOWS_MESSAGE)
	{

	}

	WindowsMessageEvent* WindowsMessageEvent::Clone() const
	{
		return new WindowsMessageEvent(*this);
    }
}
