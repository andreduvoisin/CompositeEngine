#include "application-Win32.h"

#include <Windows.h>

#ifdef _DEBUG
#include <cstdio>
#endif


namespace CE
{
	void ApplicationWin32::Initialize()
	{
#if _DEBUG
		InitializeConsole();
#endif
	}

	void ApplicationWin32::Destroy()
	{
#if _DEBUG
		DestroyConsole();
#endif
	}

#if _DEBUG
	void ApplicationWin32::InitializeConsole()
	{
		// Allocate a console.
		AllocConsole();

		// Redirect STDIN, STDOUT, STDERR to the console.
		freopen_s(&m_in, "CONIN$", "r", stdin);
		freopen_s(&m_out, "CONOUT$", "w", stdout);
		freopen_s(&m_err, "CONOUT$", "w", stderr);
	}

	void ApplicationWin32::DestroyConsole()
	{
		// Reset the console file handles to STD.
		FILE *in = stdin, *out = stdout, *err = stderr;
		freopen_s(&in, "IN", "r", m_in);
		freopen_s(&out, "OUT", "w", m_out);
		freopen_s(&err, "OUT", "w", m_err);

		// Free the console.
		FreeConsole();
	}
#endif
}
