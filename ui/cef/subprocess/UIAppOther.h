#ifndef _CE_UI_APP_OTHER_H_
#define _CE_UI_APP_OTHER_H_

#include "include/cef_app.h"

class UIAppOther : public CefApp
{
public:
    UIAppOther() = default;
    ~UIAppOther() override = default;

    UIAppOther(const UIAppOther&) = delete;
    UIAppOther(UIAppOther&&) = delete;
    UIAppOther& operator=(const UIAppOther&) = delete;
    UIAppOther& operator=(UIAppOther&&) = delete;

    // IMPLEMENT_* macros set access modifiers, so they must come last.
    IMPLEMENT_REFCOUNTING(UIAppOther);
};

#endif //_CE_UI_APP_OTHER_H_
