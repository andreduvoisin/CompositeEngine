#ifndef _CE_ASSERT_THREAD_H_
#define _CE_ASSERT_THREAD_H_

#ifdef NDEBUG


#define CE_SET_MAIN_THREAD() ((void)0)

#define CE_REQUIRE_MAIN_THREAD() ((void)0)


#else // NDEBUG

#include <thread>

namespace CE
{
	extern std::thread::id MAIN_THREAD_ID;
}

#define CE_SET_MAIN_THREAD() \
	CE::MAIN_THREAD_ID = std::this_thread::get_id()

#define CE_REQUIRE_MAIN_THREAD() \
	CE_ASSERT(CE::MAIN_THREAD_ID == std::this_thread::get_id(), "CE_REQUIRE_MAIN_THREAD() called from invalid thread.")


#endif // NDEBUG

#endif // _CE_ASSERT_THREAD_H_