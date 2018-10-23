#ifndef _CE_UI_QUERY_HANDLER_H_
#define _CE_UI_QUERY_HANDLER_H_

#include <functional>
#include <unordered_map>

#include "include/wrapper/cef_message_router.h"

enum MessageType : unsigned
{
	SAMPLE = 3
};

struct SampleMessage
{
	uint8_t dickSize;
	uint8_t dickSize2;
	uint8_t dickSize3;
	uint8_t dickSize4;
};

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
		MessageType type,
		SubscriptionCallback handler);

private:
	std::unordered_map<MessageType, std::vector<SubscriptionCallback>> registeredCallbacks;
};

#endif //_CE_UI_QUERY_HANDLER_H_
