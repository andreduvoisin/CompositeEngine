#include "UIQueryHandler.h"

#include "UIQueryResponder.h"

#include "event/SetAnimationTimeEvent.h"
#include "event/TogglePauseEvent.h"
#include "event/ToggleRenderModeEvent.h"
#include "event/core/EventSystem.h"
#include "event/PauseStateEvent.h"
#include "event/AnimationStateEvent.h"

#include "ui/message/JsonDeserializer.h"

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

	EventType type = static_cast<EventType>(deserializer.GetUint32("type"));

	UIQuery query;
	query.queryId = queryId;
	query.persistent = persistent;
	query.callback = callback;
	queryResponder->AddQuery(type, query);

	printf("Handling UI Query. EventType: %u\n", type);

	switch (type)
	{
		case EventType::TOGGLE_PAUSE:
		{
			TogglePauseEvent togglePauseEvent;
			togglePauseEvent.Deserialize(deserializer);
			eventSystem->EnqueueEvent(togglePauseEvent);
			return true;
		}

		case EventType::REQUEST_PAUSE_STATE:
		{
			RequestPauseStateEvent requestPauseStateEvent;
			requestPauseStateEvent.Deserialize(deserializer);
			eventSystem->EnqueueEvent(requestPauseStateEvent);
			return true;
		}

		case EventType::SET_ANIMATION_TIME:
		{
			SetAnimationTimeEvent setAnimationTimeEvent;
			setAnimationTimeEvent.Deserialize(deserializer);
			eventSystem->EnqueueEvent(setAnimationTimeEvent);
			return true;
		}

		case EventType::REQUEST_ANIMATION_STATE:
		{
			RequestAnimationStateEvent requestAnimationStateEvent;
			requestAnimationStateEvent.Deserialize(deserializer);
			eventSystem->EnqueueEvent(requestAnimationStateEvent);
			return true;
		}

		case EventType::TOGGLE_RENDER_MODE:
		{
			ToggleRenderModeEvent toggleRenderModeEvent;
			toggleRenderModeEvent.Deserialize(deserializer);
			eventSystem->EnqueueEvent(toggleRenderModeEvent);
			return true;
		}

		default:
		{
			// TODO: Add assert.
			printf("Unhandled UI Query. EventType: %u\n", type);
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
