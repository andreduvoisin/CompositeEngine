#include "FpsCounter.h"

#include "event/FpsStateEvent.h"
#include "event/core/EventSystem.h"
#include "clock/RealTimeClock.h"

namespace CE
{
  constexpr float FPS_SAMPLE_RATE = 0.25f;

	FpsCounter::FpsCounter(EventSystem* eventSystem)
		: eventSystem(eventSystem)
		, fps(0)
	{
	}

	void FpsCounter::Update(float deltaSeconds)
	{
    ticker_tm += deltaSeconds;
    ticker++;
    if (ticker_tm > FPS_SAMPLE_RATE) {
      fps = static_cast<uint32_t>((float)ticker / ticker_tm);
      ticker_tm = 0.0;
      ticker = 0;

      SendFpsStateEvent();
    }
	}

	void FpsCounter::SendFpsStateEvent()
	{
		uint64_t throttleTicks = RealTimeClock::Get().GetNextTicksForInterval(1.f / 30.f);

		FpsStateEvent event;
		event.fps = fps;
		eventSystem->EnqueueEventThrottled(event, throttleTicks);
	}
}
