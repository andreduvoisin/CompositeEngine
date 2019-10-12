#include "UIExternalMessagePump.h"

#include "common/debug/Assert.h"
#include "common/debug/AssertThread.h"
#include "include/cef_app.h"
#include <SDL_events.h>

#include <thread>

// Special timer delay placeholder value. Intentionally 32-bit for Windows and
// OS X platform API compatibility.
static const uint32_t TIMER_DELAY_PLACEHOLDER = (std::numeric_limits<uint32_t>::max)();

// The maximum number of milliseconds we're willing to wait between calls to
// DoWork().
static const uint32_t TIMER_DELAY_MAX = 1000 / 30; // denominator = fps

static const SDL_TimerID INVALID_TIMER_ID = 0;

static const uint32_t TIMER_EVENT = SDL_RegisterEvents(1); // NOLINT
static const uint32_t WORK_EVENT = SDL_RegisterEvents(1); // NOLINT

UIExternalMessagePump::UIExternalMessagePump()
    : timerId(INVALID_TIMER_ID)
    , active(false)
    , reentrancyDetected(false)
{
}

UIExternalMessagePump::~UIExternalMessagePump()
{
    KillTimer();
}

void UIExternalMessagePump::OnScheduleMessagePumpWork(uint32_t delayMillis)
{
    // This method may be called on any thread.
    void* data1 = reinterpret_cast<void*>(static_cast<uintptr_t>(delayMillis)); // NOLINT
    PushEvent(WORK_EVENT, data1, nullptr);
}

void UIExternalMessagePump::ProcessEvent(const SDL_Event& event)
{
    if (event.type == TIMER_EVENT)
    {
        // Timer timed out.
        OnTimerTimeout();
    }
    else if (event.type == WORK_EVENT)
    {
        // OnScheduleMessagePumpWork() request.
        auto delayMillis = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(event.user.data1)); // NOLINT
        OnScheduleWork(delayMillis);
    }
}

void UIExternalMessagePump::Shutdown()
{
    KillTimer();

    // We need to run the message pump until it is idle. However we don't have
    // that information here so we run the message loop "for a while".
    const int workIterations = 10;
    for (int i = 0; i < workIterations; ++i)
    {
        // Do some work.
        CefDoMessageLoopWork();

        // Sleep to allow the CEF proc to do work.
        const uint32_t sleepMs = 50;
        SDL_Delay(sleepMs);
    }
}

uint32_t UIExternalMessagePump::TimerCallback(uint32_t /*intervalMillis*/, void* /*param*/)
{
    // The callback is run on a separate thread.
    PushEvent(TIMER_EVENT, nullptr, nullptr);
    return 0;
}

void UIExternalMessagePump::PushEvent(uint32_t type, void* data1, void* data2)
{
    SDL_UserEvent userEvent;
    userEvent.type = type;
    userEvent.code = 0;
    userEvent.data1 = data1;
    userEvent.data2 = data2;

    SDL_Event event;
    event.type = type;
    event.user = userEvent;

    SDL_PushEvent(&event);
}

bool UIExternalMessagePump::IsTimerPending()
{
    return timerId != INVALID_TIMER_ID;
}

void UIExternalMessagePump::SetTimer(uint32_t delayMillis)
{
    CE_ASSERT(!IsTimerPending(), "Timer should not be pending on call to SetTimer().");
    CE_ASSERT(
            delayMillis > 0, "Should not set a timer with an immediate delay; instead, execute the work immediately.");
    timerId = SDL_AddTimer(delayMillis, &TimerCallback, nullptr);
}

void UIExternalMessagePump::KillTimer()
{
    if (timerId != INVALID_TIMER_ID)
    {
        SDL_RemoveTimer(timerId);
        timerId = INVALID_TIMER_ID;
    }
}

void UIExternalMessagePump::OnTimerTimeout()
{
    CE_REQUIRE_MAIN_THREAD();

    KillTimer();
    DoWork();
}

void UIExternalMessagePump::OnScheduleWork(uint32_t delayMillis)
{
    CE_REQUIRE_MAIN_THREAD();

    if (delayMillis == TIMER_DELAY_PLACEHOLDER && IsTimerPending())
    {
        // Don't set the maximum timer requested from DoWork() if a timer event is
        // currently pending.
        return;
    }

    KillTimer();

    if (delayMillis <= 0)
    {
        // Execute the work immediately.
        DoWork();
    }
    else
    {
        // Never wait longer than the maximum allowed time.
        if (delayMillis > TIMER_DELAY_MAX)
        {
            delayMillis = TIMER_DELAY_MAX;
        }

        // Results in call to OnTimerTimeout() after the specified delay.
        SetTimer(delayMillis);
    }
}

void UIExternalMessagePump::DoWork()
{
    bool wasReentrant = PerformMessageLoopWork();
    if (wasReentrant)
    {
        // Execute the remaining work as soon as possible.
        OnScheduleMessagePumpWork(0);
    }
    else if (!IsTimerPending())
    {
        // Schedule a timer event at the maximum allowed time. This may be dropped
        // in OnScheduleWork() if another timer event is already in-flight.
        OnScheduleMessagePumpWork(TIMER_DELAY_PLACEHOLDER);
    }
}

bool UIExternalMessagePump::PerformMessageLoopWork()
{
    if (active)
    {
        // When CefDoMessageLoopWork() is called there may be various callbacks
        // (such as paint and IPC messages) that result in additional calls to this
        // method. If re-entrancy is detected we must repost a request again to the
        // owner thread to ensure that the discarded call is executed in the future.
        reentrancyDetected = true;
        return false;
    }

    reentrancyDetected = false;

    active = true;
    CefDoMessageLoopWork();
    active = false;

    // |reentrancyDetected| may have changed due to re-entrant calls to this
    // method.
    return reentrancyDetected;
}
