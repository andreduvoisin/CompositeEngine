#ifndef _CE_INPUTMANAGER_WIN32_H_
#define _CE_INPUTMANAGER_WIN32_H_

#include "common\Singleton.h"

#include <unordered_map>

typedef struct tagMSG MSG;

namespace CE
{
	class InputManagerWin32 : public Singleton<InputManagerWin32>
	{
	public:
		void Initialize();
		void Destroy();

		void HandleMouseEvent(MSG msg);
		void HandleKeyboardEvent(MSG msg);

		bool GetKey(char key);

	private:
		std::unordered_map<char, bool> m_inputMap;
	};

	class InputManager : public InputManagerWin32 {};
}

#endif  // _CE_INPUTMANAGER_WIN32_H_