#include <SDL.h>

#include <string>

#include <cstdio>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* g_window = NULL;
SDL_Surface* g_surface = NULL;

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

SDL_Surface* g_keyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

SDL_Surface* g_currentSurface = NULL;

bool Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	g_window = SDL_CreateWindow(
		"Composite Engine",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (g_window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	g_surface = SDL_GetWindowSurface(g_window);

	return true;
}

void Destroy()
{
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	g_surface = NULL;

	SDL_Quit();
}

SDL_Surface* LoadSurface(std::string path)
{
	SDL_Surface* surface = SDL_LoadBMP(path.c_str());

	if (surface == NULL)
	{
		printf("Unable to load image %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}

	SDL_Surface* optimized = SDL_ConvertSurface(surface, g_surface->format, NULL);
	
	if (optimized == NULL)
	{
		printf("Unable to optimize image %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
	}

	SDL_FreeSurface(surface);

	return optimized;
}

bool LoadMedia()
{
	//Loading success flag
	bool success = true;

	//Load default surface
	g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = LoadSurface("../../../assets/press.bmp");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		printf("Failed to load default image!\n");
		success = false;
	}

	//Load up surface
	g_keyPressSurfaces[KEY_PRESS_SURFACE_UP] = LoadSurface("../../../assets/up.bmp");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		printf("Failed to load up image!\n");
		success = false;
	}

	//Load down surface
	g_keyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = LoadSurface("../../../assets/down.bmp");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		printf("Failed to load down image!\n");
		success = false;
	}

	//Load left surface
	g_keyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = LoadSurface("../../../assets/left.bmp");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		printf("Failed to load left image!\n");
		success = false;
	}

	//Load right surface
	g_keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = LoadSurface("../../../assets/right.bmp");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		printf("Failed to load right image!\n");
		success = false;
	}

	return success;
}

int main(int argc, char* argv[])
{
	if (!Initialize())
	{
		printf("Failed to initialize.\n");
		return -1;
	}

	if (!LoadMedia())
	{
		printf("Failed to load media.\n");
		return -1;
	}

	bool quit = false;
	SDL_Event event;

	g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_UP:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_UP];
							break;
						case SDLK_DOWN:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
							break;
						case SDLK_LEFT:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
							break;
						case SDLK_RIGHT:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
							break;
						default:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
							break;
					}
					break;

				case SDL_QUIT:
					quit = true;
					break;
			}
		}

		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = SCREEN_WIDTH;
		rect.h = SCREEN_HEIGHT;
		SDL_BlitScaled(g_currentSurface, NULL, g_surface, &rect);
		SDL_UpdateWindowSurface(g_window);
	}

	Destroy();

	return 0;
}
