#include "UIQueryHandler.h"

bool UIQueryHandler::OnQuery(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id,
		const CefString& request,
		bool persistent,
		CefRefPtr<Callback> callback)
{
	printf("handling message with request: %s\n", request.ToString().c_str());
	auto iterator = registeredCallbacks.find(request);
	if (iterator != registeredCallbacks.end())
	{
		printf("found handlers for request: %s\n", request.ToString().c_str());
		std::vector<SubscriptionCallback>& handlers = iterator->second;
		for (SubscriptionCallback& handler : handlers)
		{
			handler();
		}
		callback->Success("REQUEST_SUCCESS");
		return true;
	}

	printf("no registered handlers for request: %s\n", request.ToString().c_str());
	return false;
}

void UIQueryHandler::Subscribe(
	std::string id,
	SubscriptionCallback handler)
{
	printf("registering subscriber for message id: %s\n", id.c_str());
	if (registeredCallbacks.find(id) == registeredCallbacks.end())
	{
		printf("no existing handlers for message id, initializing new list: %s\n", id.c_str());
		registeredCallbacks.insert(std::make_pair(id, std::vector<SubscriptionCallback>()));
	}
	std::vector<SubscriptionCallback> &existingHandlers = registeredCallbacks.at(id);
	existingHandlers.push_back(handler);
}