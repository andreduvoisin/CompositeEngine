#ifndef _CE_UI_APP_OTHER_H_
#define _CE_UI_APP_OTHER_H_

#include "include/cef_app.h"

class UIAppOther : public CefApp
{
public:
	UIAppOther();

private:
	// IMPLEMENT_* macros set access modifiers, so they must come last.
	IMPLEMENT_REFCOUNTING(UIAppOther);
	DISALLOW_COPY_AND_ASSIGN(UIAppOther);
};

#endif //_CE_UI_APP_OTHER_H_
