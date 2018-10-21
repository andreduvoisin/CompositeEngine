#include "UIQueryHandler.h"

bool UIQueryHandler::OnQuery(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id,
		const CefString& request,
		bool persistent,
		CefRefPtr<Callback> callback)
{
	if (request == "my_request") {
		callback->Success("my_response");
		return true;
	}

	return false;
}
