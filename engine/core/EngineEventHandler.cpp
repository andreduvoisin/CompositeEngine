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
		eventSystem->RegisterListener(this, EventType::REQUEST_PAUSE_STATE);
		eventSystem->RegisterListener(this, EventType::TOGGLE_PAUSE);
		eventSystem->RegisterListener(this, EventType::TOGGLE_RENDER_MODE);
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

			case EventType::TOGGLE_RENDER_MODE:
			{
				HandleToggleRenderMode();
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

	void EngineEventHandler::HandleToggleRenderMode()
	{
		//g_renderQuad = !g_renderQuad;
		//g_renderType += 1;
		//g_renderType %= 3;

		int renderMode = engine->RenderMode();
		renderMode += 1;
		renderMode %= 3;
		engine->renderMode = renderMode;
	}
}
