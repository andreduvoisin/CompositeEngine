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

	// TODO: defaults to utf-16. should we default to utf-8?
	std::istringstream stream(request.ToString());

	MessageType messageType;
	stream.read(reinterpret_cast<char*>(&messageType), sizeof(MessageType));

	auto iterator = registeredCallbacks.find(messageType);
	if (iterator != registeredCallbacks.end())
	{
		printf("found handlers for request: %s\n", request.ToString().c_str());

		SampleMessage sampleMessage;
		stream.read(reinterpret_cast<char*>(&sampleMessage), sizeof(SampleMessage));

		std::vector<SubscriptionCallback>& handlers = iterator->second;
		for (SubscriptionCallback& handler : handlers)
		{
			handler();
		}

		std::ostringstream outputStream;
		outputStream.write(reinterpret_cast<const char*>(&sampleMessage), sizeof(SampleMessage));
		callback->Success(outputStream.str());
		return true;
	}

	printf("no registered handlers for request: %s\n", request.ToString().c_str());
	return false;
}

void UIQueryHandler::Subscribe(
	MessageType type,
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