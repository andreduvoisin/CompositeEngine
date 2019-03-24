#include "EngineEventHandler.h"

#include "Engine.h"

#include "event/PauseStateEvent.h"
#include "event/SetRenderModeEvent.h"
#include "clock/GameTimeClock.h"

namespace CE
{
	EngineEventHandler::EngineEventHandler(
			EventSystem* eventSystem,
			Engine* engine)
		: eventSystem(eventSystem)
		, engine(engine)
	{
		eventSystem->RegisterListener(this, EventType::REQUEST_PAUSE_STATE);
		eventSystem->RegisterListener(this, EventType::TOGGLE_PAUSE);
		eventSystem->RegisterListener(this, EventType::SET_RENDER_MODE);
		eventSystem->RegisterListener(this, EventType::TOGGLE_BIND_POSE);
	}

	void EngineEventHandler::OnEvent(const Event& event)
	{
		switch (event.type)
		{
			case EventType::REQUEST_PAUSE_STATE:
			{
				SendPauseStateEvent();
				break;
			}

			case EventType::TOGGLE_PAUSE:
			{
				HandleTogglePauseEvent();
				break;
			}

			case EventType::SET_RENDER_MODE:
			{
				HandleSetRenderMode(event);
				break;
			}

			case EventType::TOGGLE_BIND_POSE:
			{
				HandleToggleRenderBindPose();
				break;
			}
		}
	}

	void EngineEventHandler::SendPauseStateEvent()
	{
		PauseStateEvent pauseStateEvent;
		pauseStateEvent.paused = GameTimeClock::Get().IsPaused();
		eventSystem->EnqueueEvent(pauseStateEvent);
	}

	void EngineEventHandler::SendBindPoseStateEvent()
	{
		// PauseStateEvent pauseStateEvent;
		// pauseStateEvent.paused = engine->paused;
	 	// eventSystem->EnqueueEvent(pauseStateEvent);
	}

	void EngineEventHandler::HandleTogglePauseEvent()
	{
		GameTimeClock::Get().SetPaused(!GameTimeClock::Get().IsPaused());
		SendPauseStateEvent();
	}

	void EngineEventHandler::HandleToggleRenderBindPose()
	{
		engine->renderBindPose = !engine->renderBindPose;
		SendBindPoseStateEvent();
	}

	void EngineEventHandler::HandleSetRenderMode(const Event& event)
	{
		const SetRenderModeEvent& setRenderModeEvent = reinterpret_cast<const SetRenderModeEvent&>(event);
		engine->renderMode = setRenderModeEvent.mode;
	}
}
