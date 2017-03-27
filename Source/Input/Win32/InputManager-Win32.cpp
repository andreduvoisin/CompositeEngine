#include "InputManager-Win32.h"

#include <Windows.h>

namespace CE
{
	void InputManagerWin32::Initialize()
	{
		
	}

	void InputManagerWin32::Destroy()
	{

	}

	void InputManagerWin32::HandleMouseEvent(MSG msg)
	{

	}

	void InputManagerWin32::HandleKeyboardEvent(MSG msg)
	{
		if ((msg.wParam >= '0' && msg.wParam <= '9')
			|| (msg.wParam >= 'A' && msg.wParam <= 'Z'))
		{
			switch (msg.message)
			{
			case WM_KEYDOWN:
				m_inputMap[(char)msg.wParam] = true;
				break;
			case WM_KEYUP:
				m_inputMap[(char)msg.wParam] = false;
				break;
			}
		}
	}

	bool InputManagerWin32::GetKey(char key)
	{
		auto iter = m_inputMap.find(key);
		if (iter != m_inputMap.end())
		{
			return iter->second;
		}
		return false;
	}
}