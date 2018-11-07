#ifndef _CE_UI_QUERY_H_
#define _CE_UI_QUERY_H_

#include "event/core/EventType.h"

#include "include/wrapper/cef_message_router.h"

#include <cstdint>

struct UIQuery
{
	EventType eventType;

	int64_t queryId;
	bool persistent;
	CefRefPtr<CefMessageRouterBrowserSide::Handler::Callback> callback;
};

#endif // _CE_UI_QUERY_H_
