#ifndef _CE_UI_MESSAGE_ID_H_
#define _CE_UI_MESSAGE_ID_H_

#include <cstdint>

/**
 * A REQUEST_* always merits a RESPONSE_SUCCESS or a RESPONSE_FAILURE.
 * and is not allowed to be persistent; it will be cancelled if so.
 * Any relevant data changes will be pushed back on the SUBSCRIPTION_* channel.
 * 
 * A SUBSCRIPTION_* always merits a STATUS_* and can either be persistent
 * or nonpersistent; it will only receive one STATUS_* if nonpersistent.
 */
enum UIMessageId : uint32_t
{
	RESPONSE_SUCCESS				= 0,
	RESPONSE_FAILURE				= 1,

	REQUEST_TOGGLE_PAUSE			= 2,
	SUBSCRIPTION_PAUSE_STATE		= 3,
	STATUS_PAUSE_STATE				= 4,

	SUBSCRIPTION_ANIMATION_STATE	= 5,
	STATUS_ANIMATION_STATE			= 6
};

#endif // _CE_UI_MESSAGE_ID_H_
