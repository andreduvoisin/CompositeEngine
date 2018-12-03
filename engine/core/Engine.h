#ifndef _CE_ENGINE_H_
#define _CE_ENGINE_H_

#include "EngineEventHandler.h"

namespace CE
{
	class Engine
	{
	public:
		Engine(EventSystem* eventSystem);

		int RenderMode() const { return renderMode; }

	private:
		EngineEventHandler engineEventHandler;

		int renderMode;

		friend class EngineEventHandler;
	};
}

#endif // _CE_ENGINE_H_