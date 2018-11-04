#ifndef _CE_ENGINE_EVENT_HANDLER_H_
#define _CE_ENGINE_EVENT_HANDLER_H_

#include "event/core/EventSystem.h"
#include "event/core/EventReceiver.h"

namespace CE
{
	class Engine;

	class EngineEventHandler : public EventReceiver
	{
	public:
		EngineEventHandler(EventSystem* eventSystem, Engine* engine);

		// EventReceiver Interface
		void OnEvent(const Event& event) override;

		void SendPauseStateEvent();

	private:
		void HandleTogglePauseEvent();

		EventSystem* eventSystem;
		Engine* engine;
	};
}

#endif // _CE_ENGINE_EVENT_HANDLER_H_