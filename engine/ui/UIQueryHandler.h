#ifndef _CE_UI_QUERY_HANDLER_H_
#define _CE_UI_QUERY_HANDLER_H_

#include <functional>
#include <unordered_map>

#include "include/wrapper/cef_message_router.h"

class UIQueryHandler : public CefMessageRouterBrowserSide::Handler
{
public:
	typedef std::function<void()> SubscriptionCallback;

	bool OnQuery(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id,
		const CefString& request,
		bool persistent,
		CefRefPtr<Callback> callback) override;

	void Subscribe(
		std::string id,
		SubscriptionCallback handler);

private:
	std::unordered_map<std::string, std::vector<SubscriptionCallback>> registeredCallbacks;
};

#endif //_CE_UI_QUERY_HANDLER_H_
