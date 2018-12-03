#ifndef _CE_CLOCK_H_
#define _CE_CLOCK_H_

#include <cstdint>

namespace CE
{
	class Clock
	{
	public:
		void Initialize(uint64_t ticks);

		void Update(uint64_t deltaTicks);
		float GetDeltaSeconds() const;
		uint64_t GetNextTicksForInterval(float intervalSeconds) const;

		uint64_t GetCurrentTicks() const { return currentTicks; }
		void SetPaused(bool paused) { this->paused = paused; }
		bool IsPaused() const { return paused; }

	private:
		static float TicksToSeconds(uint64_t ticks);
		static uint64_t SecondsToTicks(float seconds);

		static uint64_t frequency;

		uint64_t currentTicks;
		uint64_t previousTicks;
		float timeScale;
		bool paused;
	};
}

#endif // _CE_CLOCK_H_