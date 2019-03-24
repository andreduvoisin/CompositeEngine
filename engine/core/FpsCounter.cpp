#include "FpsCounter.h"

#include "event/FpsStateEvent.h"
#include "event/core/EventSystem.h"
#include "clock/RealTimeClock.h"

namespace CE
{
	static const float SMOOTHING_WEIGHT = 0.1f;

	FpsCounter::FpsCounter(EventSystem* eventSystem)
		: eventSystem(eventSystem)
		, fps(0)
	{

	}

	void FpsCounter::Update(float deltaSeconds)
	{
		fps = static_cast<uint32_t>(((1.f / deltaSeconds) * SMOOTHING_WEIGHT) + (fps * (1.f - SMOOTHING_WEIGHT)));
		SendFpsStateEvent();
	}

	void FpsCounter::SendFpsStateEvent()
	{
		uint64_t throttleTicks = RealTimeClock::Get().GetNextTicksForInterval(1.f / 30.f);

		FpsStateEvent event;
		event.fps = fps;
		eventSystem->EnqueueEventThrottled(event, throttleTicks);
	}
}
