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
		eventSystem->RegisterReceiverForEvent(this, EventId::TOGGLE_RENDER_MODE);
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

			case EventId::TOGGLE_RENDER_MODE:
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
