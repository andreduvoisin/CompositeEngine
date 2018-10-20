#ifndef _CE_UI_BROWSER_PROCESS_HANDLER_H_
#define _CE_UI_BROWSER_PROCESS_HANDLER_H_

#include "include/cef_browser_process_handler.h"

class UIBrowserProcessHandler : public CefBrowserProcessHandler
{
public:
	void OnScheduleMessagePumpWork(int64 delay_ms) override;

private:

	// IMPLEMENT_* macros set access modifiers, so they must come last.
	IMPLEMENT_REFCOUNTING(UIBrowserProcessHandler);
};

#endif //_CE_UI_BROWSER_PROCESS_HANDLER_H_
