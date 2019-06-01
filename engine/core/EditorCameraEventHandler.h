#ifndef _CE_EDITOR_CAMERA_EVENT_HANDLER_H_
#define _CE_EDITOR_CAMERA_EVENT_HANDLER_H_

#include "event/core/EventListener.h"

class EventSystem;

struct SDL_MouseButtonEvent;

namespace CE
{
    class Camera;

	class EditorCameraEventHandler : public EventListener
	{
	public:
		EditorCameraEventHandler(EventSystem* eventSystem, Camera* camera);

		void Update(float deltaSeconds);

		// EventListener Interface
		void OnEvent(const Event& event) override;

	private:
		void HandleSdlEvent(const Event& event);
        void HandleMouseButtonDown(const SDL_MouseButtonEvent& event);
        void HandleMouseButtonUp(const SDL_MouseButtonEvent& event);
        void HandleWindowFocusGained();

		EventSystem* eventSystem;
		Camera* camera;

        int32_t rightClickWarpX;
        int32_t rightClickWarpY;
        bool isClickThrough;
	};
}

#endif // _CE_EDITOR_CAMERA_EVENT_HANDLER_H_