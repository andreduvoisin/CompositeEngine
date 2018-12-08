#include "AssertThread.h"

#ifdef NDEBUG





#else // NDEBUG


namespace CE
{
	std::thread::id MAIN_THREAD_ID;
}


#endif // NDEBUG
