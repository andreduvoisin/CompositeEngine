#ifndef _CE_UI_EXTERNAL_MESSAGE_PUMP_H_
#define _CE_UI_EXTERNAL_MESSAGE_PUMP_H_

#include <SDL_events.h>
#include <SDL_timer.h>

/*
 * CEF References:
 * main_message_loop_external_pump.cc
 * main_message_loop_external_pump_win.cc
 */
class UIExternalMessagePump
{
public:
    UIExternalMessagePump();
    ~UIExternalMessagePump();

    UIExternalMessagePump(const UIExternalMessagePump&) = delete;
    UIExternalMessagePump(UIExternalMessagePump&&) = delete;
    UIExternalMessagePump& operator=(const UIExternalMessagePump&) = delete;
    UIExternalMessagePump& operator=(UIExternalMessagePump&&) = delete;

    void OnScheduleMessagePumpWork(uint32_t delayMillis);
    void ProcessEvent(const SDL_Event& event);

    void Shutdown();

private:
    static uint32_t TimerCallback(uint32_t intervalMillis, void* param);
    static void PushEvent(uint32_t type, void* data1, void* data2);

    bool IsTimerPending();
    void SetTimer(uint32_t delayMillis);
    void KillTimer();
    void OnTimerTimeout();

    void OnScheduleWork(uint32_t delayMillis);
    void DoWork();
    bool PerformMessageLoopWork();

    SDL_TimerID timerId;
    bool active;
    bool reentrancyDetected;
};

#endif //_CE_UI_EXTERNAL_MESSAGE_PUMP_H_
