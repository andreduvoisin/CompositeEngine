#include "EditorCameraEventHandler.h"

#include "Camera.h"
#include "event/core/EventSystem.h"

#include "event/SdlEvent.h"

namespace CE
{
	EditorCameraEventHandler::EditorCameraEventHandler(
			EventSystem* eventSystem,
			Camera* camera)
		: eventSystem(eventSystem)
		, camera(camera)
        , rightClickWarpX(0)
        , rightClickWarpY(0)
        , isClickThrough(false)
	{
		eventSystem->RegisterListener(this, EventType::SDL);
	}

	void EditorCameraEventHandler::Update(float deltaSeconds)
	{
		int deltaMouseX, deltaMouseY;

		if (SDL_GetRelativeMouseState(&deltaMouseX, &deltaMouseY) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			float movementDelta = 500.f * deltaSeconds;

			const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

			if (keyboardState[SDL_GetScancodeFromKey(SDLK_w)])
			{
				camera->MoveForward(movementDelta);
			}
			if (keyboardState[SDL_GetScancodeFromKey(SDLK_a)])
			{
				camera->MoveLeft(movementDelta);
			}
			if (keyboardState[SDL_GetScancodeFromKey(SDLK_s)])
			{
				camera->MoveBackward(movementDelta);
			}
			if (keyboardState[SDL_GetScancodeFromKey(SDLK_d)])
			{
				camera->MoveRight(movementDelta);
			}

			bool mouseMoved = deltaMouseX != 0 || deltaMouseY != 0;

			if (mouseMoved)
			{
				// Because (0, 0) is upper left instead of lower left, we negate the delta y.
				camera->Swivel(deltaMouseX, -deltaMouseY, 0.1f * deltaSeconds);

				if (isClickThrough)
				{
					SDL_WarpMouseGlobal(rightClickWarpX, rightClickWarpY);
				}
			}
		}
	}

	void EditorCameraEventHandler::OnEvent(const Event& event)
	{
		switch (event.type)
		{
			case EventType::SDL:
			{
				HandleSdlEvent(event);
				break;
			}
		}
	}

	void EditorCameraEventHandler::HandleSdlEvent(const Event& event)
    {
		const SdlEvent& wrappedEvent = reinterpret_cast<const SdlEvent&>(event);

        const SDL_Event& nativeEvent = wrappedEvent.event;

        switch (nativeEvent.type)
        {
            case SDL_MOUSEBUTTONDOWN:
            {
                if (nativeEvent.button.button == SDL_BUTTON_RIGHT)
                {
                    SDL_SetRelativeMouseMode(SDL_TRUE);

                    SDL_GetRelativeMouseState(NULL, NULL);

                    if (isClickThrough)
                    {
                        SDL_GetGlobalMouseState(&rightClickWarpX, &rightClickWarpY);
                    }
                    else
                    {
                        rightClickWarpX = nativeEvent.button.x;
                        rightClickWarpY = nativeEvent.button.y;
                    }
                }

                break;
            }

            case SDL_MOUSEBUTTONUP:
            {
                if (nativeEvent.button.button == SDL_BUTTON_RIGHT)
                {
                    SDL_SetRelativeMouseMode(SDL_FALSE);

                    if (isClickThrough)
                    {
                        SDL_WarpMouseGlobal(rightClickWarpX, rightClickWarpY);
                        isClickThrough = false;
                    }
                    else
                    {
                        SDL_WarpMouseInWindow(NULL, rightClickWarpX, rightClickWarpY);
                    }
                }

                break;
            }

            case SDL_WINDOWEVENT:
            {
                switch (nativeEvent.window.event)
                {
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                    {
                        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
                        {
                            // This is required to warp the mouse properly.
                            isClickThrough = true;
                        }

                        break;
                    }
                }

                break;
            }
        }
    }
}
