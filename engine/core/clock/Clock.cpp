#include "Clock.h"

#include <SDL_timer.h>

namespace CE
{
	uint64_t Clock::frequency = SDL_GetPerformanceFrequency();

	void Clock::Initialize(uint64_t ticks)
	{
		currentTicks = ticks;
		previousTicks = ticks;
		timeScale = 1.f;
		paused = false;
	}

	void Clock::Update(uint64_t deltaTicks)
	{
		previousTicks = currentTicks;

		if (paused)
		{
			return;
		}

		currentTicks += static_cast<uint64_t>(deltaTicks * timeScale);
	}

	float Clock::GetDeltaSeconds() const
	{
		return TicksToSeconds(currentTicks - previousTicks);
	}

	uint64_t Clock::GetNextTicksForInterval(float intervalSeconds) const
	{
		uint64_t intervalTicks = SecondsToTicks(intervalSeconds);
		uint64_t ticksUntilNextInterval = intervalTicks - (currentTicks % intervalTicks);
		return currentTicks + ticksUntilNextInterval;
	}

	float Clock::TicksToSeconds(uint64_t ticks)
	{
		return static_cast<float>(ticks) / frequency;
	}

	uint64_t Clock::SecondsToTicks(float seconds)
	{
		return static_cast<uint64_t>(seconds * frequency);
	}
}
