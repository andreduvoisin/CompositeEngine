// Main for Win32
#ifdef __APPLE__

#include "application\Application.h"
#include "common\Defines.h"
#include "graphics\GraphicsDevice.h"
#include "graphics\MeshManager.h"
#include "input\InputManager.h"

#include <Windows.h>

#include <algorithm>


bool Initialize(HINSTANCE hInstance, int cmdShow)
{
	// Initialize our Application.
	CE::Application::Get().Initialize();

	CE::InputManager::Get().Initialize();

	// Initialize our Mesh Manager.
	CE::MeshManager::Get().Initialize();

	// Initialize our Graphics Device.
	if (!CE::GraphicsDevice::Get().Initialize(hInstance, cmdShow))
	{
		return false;
	}

	return true;
}


void Destroy()
{
	// To be safe, destroy in reverse initialize order.
	CE::GraphicsDevice::Get().Destroy();
	CE::MeshManager::Get().Destroy();
	CE::InputManager::Get().Destroy();
	CE::Application::Get().Destroy();
}


// The main application loop.
int Run()
{
	MSG msg = {0};

	static DWORD previousTime = timeGetTime();
	static const float targetFramerate = 30.0f;
	static const float maxTimeStep = 1.0f / targetFramerate;

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			// Mouse input.
			if (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)
			{
				CE::InputManager::Get().HandleMouseEvent(msg);
			}
			// Keyboard input.
			else if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
			{
				CE::InputManager::Get().HandleKeyboardEvent(msg);
			}
			// Default message case.
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			DWORD currentTime = timeGetTime();
			float deltaTime = (currentTime - previousTime) / 1000.0f;
			previousTime = currentTime;

			// Cap the delta time to the max time step (useful if you're 
			// debugging and you don't want the deltaTime value to explode).
			deltaTime = std::min<float>(deltaTime, maxTimeStep);

			CE::GraphicsDevice::Get().Update(deltaTime);

			CE::GraphicsDevice::Get().Render();
		}
	}

	return static_cast<int>(msg.wParam);
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	// Initialize.
	if (!Initialize(hInstance, cmdShow))
	{
		return -1;
	}

	// Run the application.
	int returnCode = Run();

	// Destroy.
	Destroy();

	return returnCode;
}

#endif
