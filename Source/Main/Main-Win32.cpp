// Main for Win32

#ifdef _WIN32

#include "Common/Defines.h"

#include <Windows.h>

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR cmdLine,
	int cmdShow)
{
	CE_UNUSED(hInstance);
	CE_UNUSED(hPrevInstance);
	CE_UNUSED(cmdLine);
	CE_UNUSED(cmdShow);

	return 0;
}

#endif
