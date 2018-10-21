#ifndef _CE_UI_QUERY_HANDLER_H_
#define _CE_UI_QUERY_HANDLER_H_

#include "include/wrapper/cef_message_router.h"

class UIQueryHandler : public CefMessageRouterBrowserSide::Handler
{
public:
	bool OnQuery(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id,
		const CefString& request,
		bool persistent,
		CefRefPtr<Callback> callback) override;
};

#endif //_CE_UI_QUERY_HANDLER_H_
