#ifndef _CE_ENGINE_H_
#define _CE_ENGINE_H_

#include "EngineEventHandler.h"

namespace CE
{
	class Engine
	{
	public:
		Engine(EventSystem* eventSystem);

		bool IsPaused() const { return paused; }

	private:
		EngineEventHandler engineEventHandler;

		bool paused;

		friend class EngineEventHandler;
	};
}

#endif // _CE_ENGINE_H_