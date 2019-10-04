#include "WindowContentView.h"

#include <AppKit/AppKit.h>

namespace CE
{
    void* GetWindowContentView(void* window)
    {
        auto nsWindow = (NSWindow*) window;
        return [nsWindow contentView];
    }
}
