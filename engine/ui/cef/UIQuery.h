#ifndef _CE_UI_QUERY_H_
#define _CE_UI_QUERY_H_

#include "ui/message/UIMessageId.h"

#include "include/wrapper/cef_message_router.h"

#include <cstdint>

struct UIQuery
{
	int64_t queryId;
	UIMessageId messageId;
	bool persistent;
	CefRefPtr<CefMessageRouterBrowserSide::Handler::Callback> callback;
};

#endif // _CE_UI_QUERY_H_
