#ifndef _CE_FPS_COUNTER_H_
#define _CE_FPS_COUNTER_H_

#include <cstdint>

class EventSystem;

namespace CE
{
	class FpsCounter
	{
	public:
		FpsCounter(EventSystem* eventSystem);

		void Update(float deltaTime);

	private:
		void SendFpsStateEvent();

		EventSystem* eventSystem;
		uint32_t fps;
	};
}

#endif // _CE_FPS_COUNTER_H_
