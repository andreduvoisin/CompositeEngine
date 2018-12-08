#ifndef _CE_ASSERT_H_
#define _CE_ASSERT_H_

#include <cassert>

#define CE_ASSERT(expression, message) \
	assert(expression && message)

#endif // _CE_ASSERT_H_