#include "UIQueryHandler.h"

#include "UIQueryResponder.h"

#include "message/TogglePauseMessage.h"
#include "message/AnimationStateMessage.h"
#include "message/PauseStateMessage.h"
#include "message/SetAnimationTimeMessage.h"

#include "event/SetAnimationTimeEvent.h"
#include "event/TogglePauseEvent.h"
#include "event/ToggleRenderModeEvent.h"
#include "event/core/EventSystem.h"
#include "event/PauseStateEvent.h"
#include "event/AnimationStateEvent.h"

UIQueryHandler::UIQueryHandler(
		EventSystem* eventSystem,
		UIQueryResponder* queryResponder)
	: eventSystem(eventSystem)
	, queryResponder(queryResponder)
{

}

bool UIQueryHandler::OnQuery(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 queryId,
		const CefString& request,
		bool persistent,
		CefRefPtr<Callback> callback)
{
	// TODO: Defaults to utf-16. Should we default to utf-8?
	std::string requestUtf8 = request.ToString();

	JsonDeserializer deserializer(requestUtf8.c_str());

	UIMessageId messageId = static_cast<UIMessageId>(deserializer.GetUint32("id"));

	UIQuery query;
	query.queryId = queryId;
	query.messageId = messageId;
	query.persistent = persistent;
	query.callback = callback;
	queryResponder->AddQuery(query);

	printf("Handling message, UIMessageId: %u\n", messageId);

	switch (messageId)
	{
		case UIMessageId::REQUEST_TOGGLE_PAUSE:
		{
			eventSystem->EnqueueEvent(TogglePauseEvent());
			return true;
		}

		case UIMessageId::SUBSCRIPTION_PAUSE_STATE:
		{
			eventSystem->EnqueueEvent(RequestPauseStateEvent());
			return true;
		}

		case UIMessageId::REQUEST_SET_ANIMATION_TIME:
		{
			SetAnimationTimeRequest setAnimationTimeRequest;
			setAnimationTimeRequest.Deserialize(deserializer);

			SetAnimationTimeEvent setAnimationTimeEvent;
			setAnimationTimeEvent.time = setAnimationTimeRequest.time;

			eventSystem->EnqueueEvent(setAnimationTimeEvent);
			return true;
		}

		case UIMessageId::SUBSCRIPTION_ANIMATION_STATE:
		{
			eventSystem->EnqueueEvent(RequestAnimationStateEvent());
			return true;
		}

		case UIMessageId::REQUEST_TOGGLE_RENDER_MODE:
		{
			eventSystem->EnqueueEvent(ToggleRenderModeEvent());
			return true;
		}

		default:
		{
			// TODO: Add assert.
			printf("Unhandled UIMessageId: %u\n", messageId);
		}
	}

	return false;
}

void UIQueryHandler::OnQueryCanceled(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 queryId)
{
	queryResponder->RemoveQuery(queryId);
}
