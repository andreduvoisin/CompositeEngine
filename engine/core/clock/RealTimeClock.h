#ifndef _CE_REAL_TIME_CLOCK_H_
#define _CE_REAL_TIME_CLOCK_H_

#include "Clock.h"
#include "common/Singleton.h"

namespace CE
{
	class RealTimeClock : public Clock, public Singleton<RealTimeClock> { };
}

#endif // _CE_REAL_TIME_CLOCK_H_