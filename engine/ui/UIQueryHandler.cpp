#include "UIQueryHandler.h"

#include "message/TogglePauseMessage.h"
#include "message/AnimationStateMessage.h"

bool UIQueryHandler::OnQuery(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id,
		const CefString& request,
		bool persistent,
		CefRefPtr<Callback> callback)
{
	printf("handling message with request: %s\n", request.ToString().c_str());

	// TODO: Defaults to utf-16. Should we default to utf-8?
	JsonDeserializer deserializer(request.ToString().c_str());

	UIMessageId id = static_cast<UIMessageId>(deserializer.GetUint32("id"));

	auto iterator = registeredCallbacks.find(id);
	if (iterator != registeredCallbacks.end())
	{
		printf("found handlers for request: %s\n", request.ToString().c_str());

		switch (id)
		{
			case UIMessageId::TOGGLE_PAUSE:
			{
				TogglePauseRequest togglePauseRequest;
				togglePauseRequest.Deserialize(deserializer);

				std::vector<SubscriptionCallback>& handlers = iterator->second;
				for (SubscriptionCallback& handler : handlers)
				{
					handler(&togglePauseRequest, callback);
				}

				return true;
			}

			case UIMessageId::ANIMATION_STATE:
			{
				AnimationStateRequest animationStateRequest = AnimationStateRequest::Deserialize(deserializer);

				std::vector<SubscriptionCallback>& handlers = iterator->second;
				for (SubscriptionCallback& handler : handlers)
				{
					handler(&animationStateRequest, callback);
				}

				return true;
			}
		}
	}

	printf("no registered handlers for request: %s\n", request.ToString().c_str());
	return false;
}

void UIQueryHandler::OnQueryCanceled(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id)
{

}

void UIQueryHandler::Subscribe(
		UIMessageId type,
		SubscriptionCallback handler)
{
	printf("registering subscriber for message id: %u\n", type);
	auto iterator = registeredCallbacks.find(type);
	if (iterator == registeredCallbacks.end())
	{
		printf("no existing handlers for message id, initializing new list: %u\n", type);
		iterator = registeredCallbacks.insert(std::make_pair(type, std::vector<SubscriptionCallback>())).first;
	}
	std::vector<SubscriptionCallback>& existingHandlers = iterator->second;
	existingHandlers.push_back(handler);
}
