#ifndef _CE_ANIMATION_EVENT_HANDLER_H_
#define _CE_ANIMATION_EVENT_HANDLER_H_

#include "event/core/EventSystem.h"
#include "event/core/EventListener.h"
#include "event/AnimationStateEvent.h"

namespace CE
{
	class AnimationComponent;

	class AnimationEventHandler : public EventListener
	{
	public:
		AnimationEventHandler(EventSystem* eventSystem, AnimationComponent* animationComponent);

		// EventListener Interface
		void OnEvent(const Event& event) override;

		void SendAnimationStateEvent();

	private:
		void HandleSetAnimationTimeEvent(const Event& event);

		EventSystem* eventSystem;
		AnimationComponent* animationComponent;

		uint64_t previousAnimationTimeEventTicks;
	};
}

#endif // _CE_ANIMATION_EVENT_HANDLER_H_