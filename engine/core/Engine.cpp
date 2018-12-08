#include "Engine.h"

namespace CE
{
	Engine::Engine(EventSystem* eventSystem)
		: engineEventHandler(eventSystem, this)
		, renderBindPose(false)
		, renderMode(0)
	{

	}
}
