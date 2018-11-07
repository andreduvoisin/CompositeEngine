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

	printf("Handling UI Query. EventType: %u\n", type);

	switch (type)
	{
		case EventType::TOGGLE_PAUSE:
		{
			SendEvent<TogglePauseEvent>(deserializer);
			SendSuccessResponse(callback);
			return true;
		}

		case EventType::REQUEST_PAUSE_STATE:
		{
			SendEvent<RequestPauseStateEvent>(deserializer);
			AddQueryToResponder(type, queryId, persistent, callback);
			return true;
		}

		case EventType::SET_ANIMATION_TIME:
		{
			SendEvent<SetAnimationTimeEvent>(deserializer);
			SendSuccessResponse(callback);
			return true;
		}

		case EventType::REQUEST_ANIMATION_STATE:
		{
			SendEvent<RequestAnimationStateEvent>(deserializer);
			AddQueryToResponder(type, queryId, persistent, callback);
			return true;
		}

		case EventType::TOGGLE_RENDER_MODE:
		{
			SendEvent<ToggleRenderModeEvent>(deserializer);
			SendSuccessResponse(callback);
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

void UIQueryHandler::SendSuccessResponse(CefRefPtr<Callback> callback)
{
	callback->Success("0");
}

void UIQueryHandler::AddQueryToResponder(
		EventType type,
		int64 queryId,
		bool persistent,
		CefRefPtr<Callback> callback)
{
	UIQuery query;
	query.queryId = queryId;
	query.persistent = persistent;
	query.callback = callback;
	queryResponder->AddQuery(type, query);
}
