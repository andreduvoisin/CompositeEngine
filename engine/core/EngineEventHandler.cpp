#include "EngineEventHandler.h"

#include "Engine.h"

#include "event/PauseStateEvent.h"

namespace CE
{
	EngineEventHandler::EngineEventHandler(
			EventSystem* eventSystem,
			Engine* engine)
		: eventSystem(eventSystem)
		, engine(engine)
	{
		eventSystem->RegisterReceiverForEvent(this, EventId::REQUEST_PAUSE_STATE);
		eventSystem->RegisterReceiverForEvent(this, EventId::TOGGLE_PAUSE);
	}

	void EngineEventHandler::OnEvent(const Event& event)
	{
		switch (event.id)
		{
			case EventId::REQUEST_PAUSE_STATE:
			{
				SendPauseStateEvent();
				break;
			}

			case EventId::TOGGLE_PAUSE:
			{
				HandleTogglePauseEvent();
				break;
			}
		}
	}

	void EngineEventHandler::SendPauseStateEvent()
	{
		PauseStateEvent pauseStateEvent;
		pauseStateEvent.paused = engine->paused;
		eventSystem->EnqueueEvent(pauseStateEvent);
	}

	void EngineEventHandler::HandleTogglePauseEvent()
	{
		engine->paused = !engine->paused;
		SendPauseStateEvent();
	}
}
