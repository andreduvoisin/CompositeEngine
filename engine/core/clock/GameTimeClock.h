#ifndef _CE_GAME_TIME_CLOCK_H_
#define _CE_GAME_TIME_CLOCK_H_

#include "Clock.h"
#include "common/Singleton.h"

namespace CE
{
	class GameTimeClock : public Clock, public Singleton<GameTimeClock> { };
}

#endif // _CE_GAME_TIME_CLOCK_H_