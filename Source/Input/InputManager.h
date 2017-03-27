#ifndef _CE_INPUTMANAGER_H_
#define _CE_INPUTMANAGER_H_

#if _WIN32
#include "Win32\InputManager-Win32.h"
#else
#error "Platform not supported."
#endif

#endif // _CE_INPUTMANAGER_H_