#ifndef _CE_UI_EXTERNAL_MESSAGE_PUMP_H_
#define _CE_UI_EXTERNAL_MESSAGE_PUMP_H_

#include <SDL_events.h>
#include <SDL_timer.h>

class UIExternalMessagePump
{
public:
	UIExternalMessagePump();

	void Run();
	void OnScheduleMessagePumpWork(int64_t delayMillis);
	void ProcessEvent(const SDL_Event& event);

private:
	static uint32_t TimerCallback(uint32_t intervalMillis, void* param);
	static void PushEvent(uint32_t type, void* data1);

	bool IsTimerPending();
	void SetTimer(int64_t delayMillis);
	void KillTimer();
	void OnTimerTimeout();

	void OnScheduleWork(int64_t delayMillis);
	void DoWork();
	bool PerformMessageLoopWork();

	SDL_TimerID timerId;
	bool active;
	bool reentrancyDetected;
};

#endif //_CE_UI_EXTERNAL_MESSAGE_PUMP_H_
