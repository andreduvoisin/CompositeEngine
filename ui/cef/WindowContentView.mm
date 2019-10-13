#include "WindowContentView.h"

#include <AppKit/AppKit.h>

namespace CE
{
    void* GetWindowContentView(void* window)
    {
        NSWindow* nsWindow = (NSWindow*) window;
        return [nsWindow contentView];
    }
}
