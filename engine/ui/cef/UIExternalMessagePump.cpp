#include "UIExternalMessagePump.h"

#include "include/cef_app.h"

#include <SDL_events.h>

static const int64_t TIMER_DELAY_PLACEHOLDER = (std::numeric_limits<int64_t>::max)();
static const int64_t TIMER_DELAY_MAX = 1000 / 30; // 30 fps

static const SDL_TimerID INVALID_TIMER_ID = 0;

static const uint32_t TIMER_EVENT = SDL_RegisterEvents(1);
static const uint32_t WORK_EVENT = SDL_RegisterEvents(1);

UIExternalMessagePump::UIExternalMessagePump()
	: timerId(0)
	, active(false)
	, reentrancyDetected(false)
{

}

void UIExternalMessagePump::Run()
{
	KillTimer();
	CefDoMessageLoopWork();
}

void UIExternalMessagePump::OnScheduleMessagePumpWork(int64_t delayMillis)
{
	PushEvent(WORK_EVENT, reinterpret_cast<void*>(delayMillis));
}

void UIExternalMessagePump::ProcessEvent(const SDL_Event& event)
{
	if (event.type == TIMER_EVENT)
	{
		OnTimerTimeout();
		return;
	}

	if (event.type == WORK_EVENT)
	{
		OnScheduleWork(reinterpret_cast<int64_t>(event.user.data1));
		return;
	}
}

uint32_t UIExternalMessagePump::TimerCallback(uint32_t intervalMillis, void* param)
{
	PushEvent(TIMER_EVENT, NULL);
	return 0;
}

void UIExternalMessagePump::PushEvent(uint32_t type, void* data1)
{
	SDL_UserEvent userEvent;
	userEvent.type = type;
	userEvent.code = 0;
	userEvent.data1 = data1;
	userEvent.data2 = NULL;

	SDL_Event event;
	event.type = type;
	event.user = userEvent;

	SDL_PushEvent(&event);
}

bool UIExternalMessagePump::IsTimerPending()
{
	return timerId != 0;
}

void UIExternalMessagePump::SetTimer(int64_t delayMillis)
{
	timerId = SDL_AddTimer(static_cast<uint32_t>(delayMillis), &TimerCallback, NULL);
}

void UIExternalMessagePump::KillTimer()
{
	if (timerId != INVALID_TIMER_ID) {
		SDL_RemoveTimer(timerId);
		timerId = INVALID_TIMER_ID;
	}
}

void UIExternalMessagePump::OnTimerTimeout()
{
	KillTimer();
	DoWork();
}

void UIExternalMessagePump::OnScheduleWork(int64_t delayMillis)
{
	if (delayMillis == TIMER_DELAY_PLACEHOLDER && IsTimerPending())
	{
		return;
	}

	KillTimer();

	if (delayMillis <= 0)
	{
		DoWork();
	}
	else
	{
		if (delayMillis > TIMER_DELAY_MAX)
		{
			delayMillis = TIMER_DELAY_MAX;
		}

		SetTimer(delayMillis);
	}
}

void UIExternalMessagePump::DoWork()
{
	bool wasReentrant = PerformMessageLoopWork();
	if (wasReentrant)
	{
		OnScheduleMessagePumpWork(0);
	}
	else if (!IsTimerPending())
	{
		OnScheduleMessagePumpWork(TIMER_DELAY_PLACEHOLDER);
	}
}

bool UIExternalMessagePump::PerformMessageLoopWork()
{
	if (active)
	{
		reentrancyDetected = true;
		return false;
	}

	reentrancyDetected = false;

	active = true;
	CefDoMessageLoopWork();
	active = false;

	return reentrancyDetected;
}
