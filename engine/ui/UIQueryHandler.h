#ifndef _CE_UI_QUERY_HANDLER_H_
#define _CE_UI_QUERY_HANDLER_H_

#include "message/MessageType.h"

#include "include/wrapper/cef_message_router.h"

#include <functional>
#include <unordered_map>

class UIQueryHandler : public CefMessageRouterBrowserSide::Handler
{
public:
	typedef std::function<void(void* request, CefRefPtr<Callback> callback)> SubscriptionCallback;

	bool OnQuery(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id,
		const CefString& request,
		bool persistent,
		CefRefPtr<Callback> callback) override;

	void Subscribe(
		MessageType type,
		SubscriptionCallback handler);

private:
	std::unordered_map<MessageType, std::vector<SubscriptionCallback>> registeredCallbacks;
};

#endif //_CE_UI_QUERY_HANDLER_H_
