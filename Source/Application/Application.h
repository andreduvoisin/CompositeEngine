#ifndef _CE_APPLICATION_H_
#define _CE_APPLICATION_H_

#if _WIN32
#include "Win32\Application-Win32.h"
#else
#error "Platform not supported."
#endif

#endif // _CE_APPLICATION_H_