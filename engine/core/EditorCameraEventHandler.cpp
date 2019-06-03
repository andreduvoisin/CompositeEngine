#include "EditorCameraEventHandler.h"

#include "Camera.h"
#include "event/core/EventSystem.h"

#include "event/SdlEvent.h"

namespace CE
{
    const float MOVEMENT_FACTOR = 500.f;
    const float SWIVEL_FACTOR = 0.1f;

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
		int deltaMouseX;
        int deltaMouseY;

		if (SDL_GetRelativeMouseState(&deltaMouseX, &deltaMouseY) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			float movementDelta = MOVEMENT_FACTOR * deltaSeconds;

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
			if (keyboardState[SDL_GetScancodeFromKey(SDLK_SPACE)])
			{
				camera->MoveUp(movementDelta);
			}

			bool mouseMoved = deltaMouseX != 0 || deltaMouseY != 0;

			if (mouseMoved)
			{
				// Because (0, 0) is upper left instead of lower left, we negate the delta y.
				camera->Swivel(deltaMouseX, -deltaMouseY, SWIVEL_FACTOR * deltaSeconds);

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
                HandleMouseButtonDown(nativeEvent.button);
                break;
            }

            case SDL_MOUSEBUTTONUP:
            {
                HandleMouseButtonUp(nativeEvent.button);
                break;
            }

            case SDL_WINDOWEVENT:
            {
                switch (nativeEvent.window.event)
                {
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                    {
                        HandleWindowFocusGained();
                        break;
                    }
                }

                break;
            }
        }
    }

    void EditorCameraEventHandler::HandleMouseButtonDown(const SDL_MouseButtonEvent& event)
    {
        if (event.button == SDL_BUTTON_RIGHT)
        {
            SDL_SetRelativeMouseMode(SDL_TRUE);

            SDL_GetRelativeMouseState(NULL, NULL);

            if (isClickThrough)
            {
                SDL_GetGlobalMouseState(&rightClickWarpX, &rightClickWarpY);
            }
            else
            {
                rightClickWarpX = event.x;
                rightClickWarpY = event.y;
            }
        }
    }

    void EditorCameraEventHandler::HandleMouseButtonUp(const SDL_MouseButtonEvent& event)
    {
        if (event.button == SDL_BUTTON_RIGHT)
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
    }

    void EditorCameraEventHandler::HandleWindowFocusGained()
    {
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
        {
            // This is required to warp the mouse properly.
            isClickThrough = true;
        }
    }
}
