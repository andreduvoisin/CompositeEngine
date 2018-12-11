#ifndef _CE_UI_BROWSER_PROCESS_HANDLER_H_
#define _CE_UI_BROWSER_PROCESS_HANDLER_H_

#include "include/cef_browser_process_handler.h"

class UIExternalMessagePump;

class UIBrowserProcessHandler : public CefBrowserProcessHandler
{
public:
	UIBrowserProcessHandler(UIExternalMessagePump* externalMessagePump);

	// CefBrowserProcessHandler Interface
	void OnScheduleMessagePumpWork(int64 delayMillis) override;

private:
	uint32_t ClampToUint32(int64 value);

	UIExternalMessagePump* externalMessagePump;

	// IMPLEMENT_* macros set access modifiers, so they must come last.
	IMPLEMENT_REFCOUNTING(UIBrowserProcessHandler);
};

#endif //_CE_UI_BROWSER_PROCESS_HANDLER_H_
