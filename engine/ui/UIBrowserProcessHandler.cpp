#include "UIBrowserProcessHandler.h"

// todo: probably too much include here lol
#include "SDL.h"
#include "include/cef_app.h"

void UIBrowserProcessHandler::OnScheduleMessagePumpWork(int64 delay_ms)
{
	SDL_Event event;
	SDL_UserEvent userEvent;
	userEvent.type = SDL_USEREVENT;
	userEvent.code = 0;
	userEvent.data1 = NULL;
	userEvent.data2 = NULL;
	event.type = SDL_USEREVENT;
	event.user = userEvent;
	SDL_PushEvent(&event);
}
