#ifndef _CE_GRAPHICS_DEVICE_H_
#define _CE_GRAPHICS_DEVICE_H_

#if _WIN32
#include "win32\GraphicsDevice-Win32.h"
#else
#error "Platform not supported."
#endif

#endif // _CE_GRAPHICS_DEVICE_H_