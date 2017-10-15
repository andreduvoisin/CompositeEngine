#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include <cstdio>

#include "graphics\MeshManager.h"
#include "graphics\MeshData.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

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
//GLint g_vertexPos2DLocation = -1;
GLuint g_vbo = 0;
GLuint g_ibo = 0;
GLuint g_vao = 0;

GLuint g_mvpMatrixID = -1;
GLuint g_paletteID = -1;
GLuint g_paletteTextureUnit = -1;
GLuint g_diffuseTextureID = -1;
GLuint g_diffuseTextureUnit = -1;

const char* g_fbxName = "..\\..\\..\\assets\\Soldier_animated_jump.fbx";

void printProgramLog(GLuint program)
{
	//Make sure name is shader
	if (!glIsProgram(program))
	{
		printf("Name %d is not a program\n", program);
		return;
	}

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

void printShaderLog(GLuint shader)
{
	//Make sure name is shader
	if (!glIsShader(shader))
	{
		printf("Name %d is not a shader\n", shader);
		return;
	}

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render quad
	//if (g_renderQuad)
	//{
	//	//Bind program
	//	glUseProgram(g_programID);

	//	//Enable vertex position
	//	glEnableVertexAttribArray(g_vertexPos2DLocation);

	//	//Set vertex data
	//	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	//	glVertexAttribPointer(g_vertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

	//	//Set index data and render
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
	//	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

	//	//Disable vertex position
	//	glDisableVertexAttribArray(g_vertexPos2DLocation);

	//	//Unbind program
	//	glUseProgram(NULL);
	//}

	glUseProgram(g_programID);

	glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 400), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mvp = projection * view * model;
	glUniformMatrix4fv(g_mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);

	CE::MeshData* meshData = CE::MeshManager::Get().GetMeshData(g_fbxName);
	glActiveTexture(GL_TEXTURE0 + g_paletteTextureUnit);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, meshData->m_palette.size(), 0, GL_RGBA32F, GL_FLOAT, meshData->m_palette.data());
	//glUniform1i(g_paletteID, g_paletteTextureUnit);

	glBindVertexArray(g_vao);

	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawArrays(GL_TRIANGLES, 0, meshData->m_vertices.size());
	glDrawElements(GL_TRIANGLES, meshData->m_indices.size(), GL_UNSIGNED_INT, NULL);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

bool InitializeOpenGL()
{
	g_programID = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//const GLchar* vertexShaderSource[] =
	//{
	//	"#version 410\nin vec2 LVertexPos2D; void main() { gl_Position = vec4(LVertexPos2D.x, LVertexPos2D.y, 0, 1); }"
	//};
	const char* vertexShaderSource =
		"#version 410\n"
		"in vec3 vp;"
		"in vec2 tp;"
		"in float jointWeights[4];"
		"in unsigned int jointIndices[4];"
		"in unsigned int numWeights;" // TODO: remove. always 4
		"uniform mat4 mvp;"
		"uniform sampler2D palette;"
		"out vec2 texCoord;"
		"void main() {"
		"  mat4 texturePalette = mat4("
		"    texture(palette, vec2(0.00, float(30 - jointIndices[0]) / 31.0)),"
		"    texture(palette, vec2(0.25, float(30 - jointIndices[0]) / 31.0)),"
		"    texture(palette, vec2(0.50, float(30 - jointIndices[0]) / 31.0)),"
		"    texture(palette, vec2(0.75, float(30 - jointIndices[0]) / 31.0)));"
		"  vec4 position = (texturePalette * vec4(vp, 1.0)) * jointWeights[0];"
		"  texturePalette = mat4("
		"    texture(palette, vec2(0.00, float(30 - jointIndices[1]) / 31.0)),"
		"    texture(palette, vec2(0.25, float(30 - jointIndices[1]) / 31.0)),"
		"    texture(palette, vec2(0.50, float(30 - jointIndices[1]) / 31.0)),"
		"    texture(palette, vec2(0.75, float(30 - jointIndices[1]) / 31.0)));"
		"  position += (texturePalette * vec4(vp, 1.0)) * jointWeights[1];"
		"  texturePalette = mat4("
		"    texture(palette, vec2(0.00, float(30 - jointIndices[2]) / 31.0)),"
		"    texture(palette, vec2(0.25, float(30 - jointIndices[2]) / 31.0)),"
		"    texture(palette, vec2(0.50, float(30 - jointIndices[2]) / 31.0)),"
		"    texture(palette, vec2(0.75, float(30 - jointIndices[2]) / 31.0)));"
		"  position += (texturePalette * vec4(vp, 1.0)) * jointWeights[2];"
		"  texturePalette = mat4("
		"    texture(palette, vec2(0.00, float(30 - jointIndices[3]) / 31.0)),"
		"    texture(palette, vec2(0.25, float(30 - jointIndices[3]) / 31.0)),"
		"    texture(palette, vec2(0.50, float(30 - jointIndices[3]) / 31.0)),"
		"    texture(palette, vec2(0.75, float(30 - jointIndices[3]) / 31.0)));"
		"  position += (texturePalette * vec4(vp, 1.0)) * jointWeights[3];"
		"  gl_Position = mvp * position;"
		"  texCoord = tp;"
		"}";
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
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
	//const GLchar* fragmentShaderSource[] =
	//{
	//	"#version 410\nout vec4 LFragment; void main() { LFragment = vec4(1.0, 1.0, 1.0, 1.0); }"
	//};
	const char* fragmentShaderSource =
		"#version 410\n"
		"out vec4 frag_colour;"
		"in vec2 texCoord;"
		"uniform sampler2D ourTexture;"
		"void main() {"
		"  frag_colour = texture(ourTexture, texCoord);"
		"}";
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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

	g_mvpMatrixID = glGetUniformLocation(g_programID, "mvp");
	g_paletteID = glGetUniformLocation(g_programID, "palette");
	g_diffuseTextureID = glGetUniformLocation(g_programID, "ourTexture");

	//g_vertexPos2DLocation = glGetAttribLocation(g_programID, "LVertexPos2D");
	//if (g_vertexPos2DLocation == -1)
	//{
	//	printf("LVertexPos2D is not a valid glsl program variable!\n");
	//	return false;
	//}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	const GLfloat vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
	};

	const unsigned int index_buffer_data[] = {
		0, 1, 2,
		0, 2, 3,
	};

	//const GLfloat vertex_buffer_data[] = {
	//	-0.500000, -0.500000, 0.000000,
	//	-0.500000, 0.500000, 0.000000,
	//	0.500000, 0.500000, 0.000000,
	//	0.500000, -0.500000, 0.000000,
	//	-0.500000, -0.500000, 1.000000,
	//	0.500000, -0.500000, 1.000000,
	//	0.500000, 0.500000, 1.000000,
	//	-0.500000, 0.500000, 1.000000,
	//};

	//const unsigned int index_buffer_data[] = {
	//	0, 1, 2,
	//	0, 2, 3,
	//	4, 5, 6,
	//	4, 6, 7,
	//	0, 3, 5,
	//	0, 5, 4,
	//	3, 2, 6,
	//	3, 6, 5,
	//	2, 1, 7,
	//	2, 7, 6,
	//	1, 0, 4,
	//	1, 4, 7,
	//};

	// "I'm Batman."
	CE::MeshData* meshData = CE::MeshManager::Get().GetMeshData(g_fbxName);

	int vertexSize = sizeof(float) * 3 + sizeof(float) * 2 + sizeof(float) * 4 + sizeof(unsigned) * 4 + sizeof(unsigned) * 1;

	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	glGenBuffers(1, &g_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glBufferData(GL_ARRAY_BUFFER, meshData->m_vertices.size() * vertexSize, meshData->m_vertices.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &g_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData->m_indices.size() * sizeof(unsigned int), meshData->m_indices.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), index_buffer_data, GL_STATIC_DRAW);

	stbi_set_flip_vertically_on_load(true);
	int width, height, channels;
	unsigned char* data = stbi_load(meshData->m_diffuseMapName.c_str(), &width, &height, &channels, 0);

	unsigned texture;

	g_diffuseTextureUnit = 0;
	glActiveTexture(GL_TEXTURE0 + g_diffuseTextureUnit);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned int glChannels = channels == 3 ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, glChannels, width, height, 0, glChannels, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glUniform1i(g_diffuseTextureID, 0);

	stbi_image_free(data);

	g_paletteTextureUnit = 1;
	glActiveTexture(GL_TEXTURE0 + g_paletteTextureUnit);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glUniform1i(g_paletteID, g_paletteTextureUnit);

	unsigned int stride = vertexSize;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(CE::Position));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(CE::Position) + sizeof(CE::TextureCoordinate)));
	glVertexAttribPointer(3, 4, GL_UNSIGNED_INT, GL_FALSE, stride, (void*)(sizeof(CE::Position) + sizeof(CE::TextureCoordinate) + sizeof(float) * 4));
	glVertexAttribPointer(4, 1, GL_UNSIGNED_INT, GL_FALSE, stride, (void*)(sizeof(CE::Position) + sizeof(CE::TextureCoordinate) + sizeof(float) * 4 + sizeof(unsigned) * 4));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	//for (int i = 0; i < meshData->m_vertices.size(); ++i)
	//{
	//	//const CE::Position& pos = meshData->m_vertices[i].position;
	//	//printf("x: %f, y: %f, z: %f\n", pos.x, pos.y, pos.z);
	//	const CE::TextureCoordinate& tex = meshData->m_vertices[i].textureCoordinate;
	//	printf("u: %f, v: %f\n", tex.u, tex.v);
	//}

	//for (int i = 0; i < meshData->m_indices.size(); i += 3)
	//{
	//	printf("idx: %i, %i, %i\n", meshData->m_indices[i], meshData->m_indices[i+1], meshData->m_indices[i+2]);
	//}

	printf("meshData->m_vertices.size(): %u\n", meshData->m_vertices.size());
	printf("meshData->m_indices.size(): %u\n", meshData->m_indices.size());

	return true;
}

bool Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

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

	CE::MeshManager::Get().Initialize();

	if (!InitializeOpenGL())
	{
		printf("Unable to initialize OpenGL!\n");
		return false;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("GL_RENDERER: %s\n", renderer);
	printf("GL_VERSION: %s\n", version);

	GLint components;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &components);
	printf("GL_MAX_VERTEX_UNIFORM_COMPONENTS: %u\n", components);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	return true;
}

void Destroy()
{
	CE::MeshManager::Get().Destroy();

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
		getchar();
		return -1;
	}

	if (!LoadMedia())
	{
		printf("Failed to load media.\n");
		getchar();
		return -1;
	}

	bool quit = false;
	SDL_Event event;

	//g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

	SDL_StartTextInput();

	unsigned long long now = SDL_GetPerformanceCounter();
	unsigned long long last = 0;
	float deltaTime = 0;

	while (!quit)
	{
		last = now;
		now = SDL_GetPerformanceCounter();
		deltaTime = double((now - last) * 1000) / SDL_GetPerformanceFrequency();

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

		CE::MeshData* meshData = CE::MeshManager::Get().GetMeshData(g_fbxName);
		meshData->Update(deltaTime);

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
