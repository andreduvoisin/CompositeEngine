// Main for Win32

/*
** https://www.3dgep.com/introduction-to-directx-11/
** https://www.3dgep.com/introduction-to-opengl-and-glsl/
** http://www.walkerb.net/blog/dx-4/
*/

#ifdef _WIN32

#include "Common\Defines.h"

#include "Graphics\GraphicsDevice.h"
#include "Graphics\MeshManager.h"

#include <Windows.h>

#include <algorithm>

// Link library dependencies.
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")


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
			TranslateMessage(&msg);
			DispatchMessage(&msg);
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

	// Initialize our Mesh Manager.
	CE::MeshManager::Get().Initialize();

	if (!CE::MeshManager::Get().GetMeshData("..\\..\\Assets\\bat.fbx"))
	{
		MessageBox(nullptr, TEXT("RIP Batman"), TEXT("Error"), MB_OK);
		return -1;
	}

	// Initialize our Graphics Device.
	if (!CE::GraphicsDevice::Get().Initialize(hInstance, cmdShow))
	{
		return -1;
	}

	int returnCode = Run();

	CE::GraphicsDevice::Get().Destroy();
	CE::MeshManager::Get().Destroy();

	return returnCode;
}

#endif
