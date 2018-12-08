#ifndef _CE_ENGINE_EVENT_HANDLER_H_
#define _CE_ENGINE_EVENT_HANDLER_H_

#include "event/core/EventSystem.h"
#include "event/core/EventListener.h"

namespace CE
{
	class Engine;

	class EngineEventHandler : public EventListener
	{
	public:
		EngineEventHandler(EventSystem* eventSystem, Engine* engine);

		// EventListener Interface
		void OnEvent(const Event& event) override;

		void SendPauseStateEvent();

	private:
		void HandleTogglePauseEvent();
		void HandleSetRenderMode(const Event& event);
		void HandleToggleRenderBindPose();

		EventSystem* eventSystem;
		Engine* engine;
	};
}

#endif // _CE_ENGINE_EVENT_HANDLER_H_