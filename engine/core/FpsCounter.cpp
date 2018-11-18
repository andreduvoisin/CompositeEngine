#include "FpsCounter.h"

#include "event/FpsStateEvent.h"
#include "event/core/EventSystem.h"

namespace CE
{
	static const float SMOOTHING_WEIGHT = 0.1f;

	FpsCounter::FpsCounter(EventSystem* eventSystem)
		: eventSystem(eventSystem)
		, fps(0)
	{

	}

	void FpsCounter::Update(float deltaTime)
	{
		fps = static_cast<uint32_t>(((1000.0f / deltaTime) * SMOOTHING_WEIGHT) + (fps * (1.0f - SMOOTHING_WEIGHT)));
		SendFpsStateEvent();
	}

	void FpsCounter::SendFpsStateEvent()
	{
		FpsStateEvent event;
		event.fps = fps;
		eventSystem->EnqueueEvent(event);
	}
}
