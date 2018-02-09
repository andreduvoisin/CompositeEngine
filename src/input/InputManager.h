#ifndef _CE_INPUT_MANAGER_H_
#define _CE_INPUT_MANAGER_H_

#if _WIN32
#include "win32\InputManager-Win32.h"
#else
#error "Platform not supported."
#endif

#endif // _CE_INPUT_MANAGER_H_