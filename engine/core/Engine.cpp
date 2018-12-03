#include "Engine.h"

namespace CE
{
	Engine::Engine(EventSystem* eventSystem)
		: engineEventHandler(eventSystem, this)
		, renderMode(0)
	{

	}
}
