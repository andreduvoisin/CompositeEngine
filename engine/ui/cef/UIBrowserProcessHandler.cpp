#include "UIBrowserProcessHandler.h"

#include "UIExternalMessagePump.h"

UIBrowserProcessHandler::UIBrowserProcessHandler(UIExternalMessagePump* externalMessagePump)
	: externalMessagePump(externalMessagePump)
{
	
}

void UIBrowserProcessHandler::OnScheduleMessagePumpWork(int64 delayMillis)
{
	externalMessagePump->OnScheduleMessagePumpWork(delayMillis);
}
