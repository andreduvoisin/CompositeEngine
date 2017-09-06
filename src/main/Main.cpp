#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <gl\GLU.h>

#include <string>

#include <cstdio>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* g_window = NULL;
SDL_Surface* g_surface = NULL;
SDL_GLContext g_context;

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

bool g_renderQuad = true;

GLuint g_programID = 0;
GLint g_vertexPos2DLocation = -1;
GLuint g_vbo = 0;
GLuint g_ibo = 0;

void printProgramLog(GLuint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}

void printShaderLog(GLuint shader)
{
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}

void HandleKeys(unsigned char key, int x, int y)
{
	if (key == 'q')
	{
		g_renderQuad = !g_renderQuad;
	}
}

void Update()
{
	(void)0;
}

void Render()
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//Render quad
	if (g_renderQuad)
	{
		//Bind program
		glUseProgram(g_programID);

		//Enable vertex position
		glEnableVertexAttribArray(g_vertexPos2DLocation);

		//Set vertex data
		glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
		glVertexAttribPointer(g_vertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

		//Set index data and render
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

		//Disable vertex position
		glDisableVertexAttribArray(g_vertexPos2DLocation);

		//Unbind program
		glUseProgram(NULL);
	}

	// OpenGL 2.1
	/*
	glClear(GL_COLOR_BUFFER_BIT);

	if (g_renderQuad)
	{
		glBegin(GL_QUADS);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.5f);
		glEnd();
	}
	*/
}

bool InitializeOpenGL()
{
	g_programID = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexShaderSource[] =
	{
		"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4(LVertexPos2D.x, LVertexPos2D.y, 0, 1); }"
	};
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		printShaderLog(vertexShader);
		return false;
	}
	glAttachShader(g_programID, vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSource[] =
	{
		"#version 140\nout vec4 LFragment; void main() { LFragment = vec4(1.0, 1.0, 1.0, 1.0); }"
	};
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
	if (fShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile fragment shader %d!\n", fragmentShader);
		printShaderLog(fragmentShader);
		return false;
	}
	glAttachShader(g_programID, fragmentShader);

	glLinkProgram(g_programID);
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(g_programID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		printf("Error linking program %d!\n", g_programID);
		printProgramLog(g_programID);
		return false;
	}

	g_vertexPos2DLocation = glGetAttribLocation(g_programID, "LVertexPos2D");
	if (g_vertexPos2DLocation == -1)
	{
		printf("LVertexPos2D is not a valid glsl program variable!\n");
		return false;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat vertexData[] =
	{
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f,  0.5f,
		-0.5f,  0.5f
	};

	GLuint indexData[] = { 0, 1, 2, 3 };

	glGenBuffers(1, &g_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
	
	glGenBuffers(1, &g_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

	return true;

	// OpenGL 2.1
	/*
	GLenum error = GL_NO_ERROR;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		return false;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		return false;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		return false;
	}

	return true;
	*/
}

bool Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Use OpenGL 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	g_window = SDL_CreateWindow(
		"Composite Engine",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (g_window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	g_surface = SDL_GetWindowSurface(g_window);

	g_context = SDL_GL_CreateContext(g_window);

	if (g_context == NULL)
	{
		printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Initialize GLEW
	//glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
	}

	// Use VSync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		printf("Warning: Unable to set VSync! SDL_Error: %s\n", SDL_GetError());
	}

	if (!InitializeOpenGL())
	{
		printf("Unable to initialize OpenGL!\n");
		return false;
	}

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

	//g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

	SDL_StartTextInput();

	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					//switch (event.key.keysym.sym)
					//{
					//	case SDLK_UP:
					//		g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_UP];
					//		break;
					//	case SDLK_DOWN:
					//		g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
					//		break;
					//	case SDLK_LEFT:
					//		g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
					//		break;
					//	case SDLK_RIGHT:
					//		g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
					//		break;
					//	default:
					//		//g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
					//		break;
					//}
					break;

				case SDL_TEXTINPUT:
				{
					int x = 0, y = 0;
					SDL_GetMouseState(&x, &y);
					HandleKeys(event.text.text[0], x, y);
					break;
				}

				case SDL_QUIT:
					quit = true;
					break;
			}
		}

		Render();
		SDL_GL_SwapWindow(g_window);

		//SDL_Rect rect;
		//rect.x = 0;
		//rect.y = 0;
		//rect.w = SCREEN_WIDTH;
		//rect.h = SCREEN_HEIGHT;
		//SDL_BlitScaled(g_currentSurface, NULL, g_surface, &rect);
		//SDL_UpdateWindowSurface(g_window);
	}

	SDL_StopTextInput();

	Destroy();

	return 0;
}
