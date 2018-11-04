#ifndef _CE_ANIMATION_EVENT_HANDLER_H_
#define _CE_ANIMATION_EVENT_HANDLER_H_

#include "event/core/EventSystem.h"
#include "event/core/EventReceiver.h"
#include "event/AnimationStateEvent.h"

namespace CE
{
	class AnimationComponent;

	class AnimationEventHandler : public EventReceiver
	{
	public:
		AnimationEventHandler(EventSystem* eventSystem, AnimationComponent* animationComponent);

		// EventReceiver Interface
		void OnEvent(const Event& event) override;

		void SendAnimationStateEvent();

	private:
		void HandleSetAnimationTimeEvent(const Event& event);

		EventSystem* eventSystem;
		AnimationComponent* animationComponent;
	};
}

#endif // _CE_ANIMATION_EVENT_HANDLER_H_