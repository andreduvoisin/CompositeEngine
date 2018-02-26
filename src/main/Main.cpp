#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <string>
#include <cstdio>
#include <fstream>
#include <sstream>

#include "graphics\MeshManager.h"
#include "graphics\AnimationManager.h"
#include "graphics\SkeletonManager.h"
#include "graphics\TextureManager.h"
#include "graphics\Mesh.h"
#include "graphics\Texture.h"
#include "graphics\Vertex.h"

#include "graphics\fbx\FBXImporter.h"
#include "graphics\STBImageImporter.h"

#include "graphics\MeshComponent.h"
#include "graphics\AnimationComponent.h"

#include "graphics\AnimationOptimizer.h"

#include "graphics\Skeleton.h"
#include <glm\gtx\matrix_decompose.hpp>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

SDL_Window* g_window = NULL;
SDL_GLContext g_context;

bool g_renderQuad = true;
bool g_renderBindPose = false;

GLuint g_programID = 0;
GLuint g_vbo = 0;
GLuint g_ibo = 0;
GLuint g_vao = 0;
GLuint g_tbo = 0;

GLuint g_programID2 = 0;

GLuint g_projectionViewModelMatrixID = -1;
GLuint g_paletteID = -1;
GLuint g_paletteTextureUnit = -1;
GLuint g_paletteGenTex = -1;
GLuint g_diffuseTextureLocation = -1;
GLuint g_diffuseTextureUnit = -1;
GLuint g_diffuseTextureID = -1;

GLuint g_projectionViewModelMatrixID2 = -1;
GLuint g_paletteID2 = -1;

//const char* g_fbxName = "..\\..\\..\\assets\\Stand Up.fbx";
//const char* g_fbxName = "..\\..\\..\\assets\\Soldier_animated_jump.fbx";
const char* g_fbxName = "..\\..\\..\\assets\\Thriller Part 2.fbx";

CE::FBXImporter* g_fbxImporter;
CE::STBImageImporter* g_stbiImporter;

CE::MeshComponent* g_meshComponent;
CE::AnimationComponent* g_animationComponent;

int g_renderType = 0;

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
		g_renderType += 1;
		g_renderType %= 3;
	}

	if (key == 'w')
	{
		g_renderBindPose = !g_renderBindPose;
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

	glUseProgram(g_programID);
	glBindVertexArray(g_vao);

	unsigned int stride = sizeof(CE::Vertex);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(CE::Position));
	glVertexAttribIPointer(2, 1, GL_INT, stride, (void*)(sizeof(CE::Position) + sizeof(CE::TextureCoordinate)));
	glVertexAttribIPointer(3, 1, GL_INT, stride, (void*)(sizeof(CE::Position) + sizeof(CE::TextureCoordinate) + sizeof(int) * 1));
	glVertexAttribIPointer(4, 1, GL_INT, stride, (void*)(sizeof(CE::Position) + sizeof(CE::TextureCoordinate) + sizeof(int) * 2));
	glVertexAttribIPointer(5, 1, GL_INT, stride, (void*)(sizeof(CE::Position) + sizeof(CE::TextureCoordinate) + sizeof(int) * 3));
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(CE::Position) + sizeof(CE::TextureCoordinate) + sizeof(int) * 4));
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(CE::Position) + sizeof(CE::TextureCoordinate) + sizeof(int) * 4 + sizeof(float) * 1));
	glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(CE::Position) + sizeof(CE::TextureCoordinate) + sizeof(int) * 4 + sizeof(float) * 2));
	glVertexAttribPointer(9, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(CE::Position) + sizeof(CE::TextureCoordinate) + sizeof(int) * 4 + sizeof(float) * 3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);
	glEnableVertexAttribArray(8);
	glEnableVertexAttribArray(9);

	glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
	//glm::mat4 view = glm::lookAt(glm::vec3(0, 100, 400), glm::vec3(0, 100, 0), glm::vec3(0, 1, 0)); // paladin
	//glm::mat4 view = glm::lookAt(glm::vec3(0, 200, 400), glm::vec3(0, 100, 0), glm::vec3(0, 1, 0)); // solider
	glm::mat4 view = glm::lookAt(glm::vec3(0, 200, 700), glm::vec3(0, 50, 0), glm::vec3(0, 1, 0)); // thriller
	//glm::mat4 view = glm::lookAt(glm::vec3(0, 2, 8), glm::vec3(0, 2, 0), glm::vec3(0, 1, 0)); // wonder woman
	//glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projectionViewModel = projection * view * model;
	glUniformMatrix4fv(g_projectionViewModelMatrixID, 1, GL_FALSE, &projectionViewModel[0][0]);

	if (g_renderBindPose)
	{
		g_animationComponent->ResetMatrixPalette();
	}

	g_animationComponent->BindMatrixPalette(
		g_paletteTextureUnit, 
		g_paletteGenTex,
		g_tbo,
		g_paletteID);

	if (g_renderType == 0 || g_renderType == 2)
	{
		g_meshComponent->Draw(
			g_vbo,
			g_ibo,
			g_diffuseTextureID,
			g_diffuseTextureLocation,
			g_diffuseTextureUnit);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
	glDisableVertexAttribArray(7);
	glDisableVertexAttribArray(8);
	glDisableVertexAttribArray(9);


	glUseProgram(g_programID2);

	stride = 28;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(12));
	glVertexAttribIPointer(2, 1, GL_INT, stride, (void*)(24));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glUniformMatrix4fv(g_projectionViewModelMatrixID2, 1, GL_FALSE, &projectionViewModel[0][0]);
	g_animationComponent->BindMatrixPalette(
		g_paletteTextureUnit,
		g_paletteGenTex,
		g_tbo,
		g_paletteID2);

	struct DebugSkeletonVertex
	{
		glm::vec3 position;
		glm::vec3 color;
		int jointIndex;
	};

	CE::Skeleton* skeleton = CE::SkeletonManager::Get().GetSkeleton(g_fbxName);

	std::vector<DebugSkeletonVertex> debugVertices;
	std::vector<unsigned> debugJointIndices;
	std::vector<unsigned> debugLineIndices;

	for (unsigned i = 0; i < skeleton->joints.size(); ++i)
	{
		glm::mat4 bindPose = glm::inverse(skeleton->joints[i].inverseBindPose);

		// todo: bad
		glm::vec3 scale;
		glm::quat orientation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(
			bindPose,
			scale,
			orientation,
			translation,
			skew,
			perspective);

		DebugSkeletonVertex vertex;
		vertex.position = translation;
		vertex.color = glm::vec3(1.f, 0.f, 0.f);
		vertex.jointIndex = i;

		debugVertices.push_back(vertex);

		debugJointIndices.push_back(i);

		if (skeleton->joints[i].parentIndex != -1)
		{
			debugLineIndices.push_back(skeleton->joints[i].parentIndex);
			debugLineIndices.push_back(i);
		}
	}

	if (g_renderType == 1 || g_renderType == 2)
	{
		glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(DebugSkeletonVertex) * debugVertices.size(), debugVertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * debugJointIndices.size(), debugJointIndices.data(), GL_STATIC_DRAW);

		glPointSize(5.f);
		glDrawElements(GL_POINTS, debugJointIndices.size(), GL_UNSIGNED_INT, NULL);

		for (unsigned i = 0; i < skeleton->joints.size(); ++i)
		{
			debugVertices[i].color = glm::vec3(1.f, 1.f, 0.f);
		}

		glBufferData(GL_ARRAY_BUFFER, sizeof(DebugSkeletonVertex) * debugVertices.size(), debugVertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * debugLineIndices.size(), debugLineIndices.data(), GL_STATIC_DRAW);

		glLineWidth(1.f);
		glDrawElements(GL_LINES, debugLineIndices.size(), GL_UNSIGNED_INT, NULL);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

std::string ReadFile(const char* file)
{
	std::ifstream stream(file);

	if (!stream.is_open())
	{
		printf("Error Opening File: %s\n", file);
		return std::string();
	}

	std::stringstream buffer;
	buffer << stream.rdbuf();
	return buffer.str();
}

bool CreateProgram2()
{
	g_programID2 = glCreateProgram();

	// TODO: Copy shaders in CMAKE to .exe dir (or subdir next to .exe).

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderSource = ReadFile("..\\..\\..\\src\\shaders\\SkeletonShader.vert");
	const char* vertexShaderSourceStr = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSourceStr, NULL);
	glCompileShader(vertexShader);
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	printShaderLog(vertexShader);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		return false;
	}
	glAttachShader(g_programID2, vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//const GLchar* fragmentShaderSource[] =
	//{
	//	"#version 410\nout vec4 LFragment; void main() { LFragment = vec4(1.0, 1.0, 1.0, 1.0); }"
	//};
	std::string fragmentShaderSource = ReadFile("..\\..\\..\\src\\shaders\\FragmentShader.frag");
	const char* fragmentShaderSourceStr = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceStr, NULL);
	glCompileShader(fragmentShader);
	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
	printShaderLog(fragmentShader);
	if (fShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile fragment shader %d!\n", fragmentShader);
		return false;
	}
	glAttachShader(g_programID2, fragmentShader);

	glLinkProgram(g_programID2);
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(g_programID2, GL_LINK_STATUS, &programSuccess);
	printProgramLog(g_programID2);
	if (programSuccess != GL_TRUE)
	{
		printf("Error linking program %d!\n", g_programID2);
		return false;
	}

	return true;
}

bool InitializeOpenGL()
{
	g_programID = glCreateProgram();

	// TODO: Copy shaders in CMAKE to .exe dir (or subdir next to .exe).

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderSource = ReadFile("..\\..\\..\\src\\shaders\\SkinnedMeshShader.vert");
	const char* vertexShaderSourceStr = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSourceStr, NULL);
	glCompileShader(vertexShader);
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	printShaderLog(vertexShader);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		return false;
	}
	glAttachShader(g_programID, vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//const GLchar* fragmentShaderSource[] =
	//{
	//	"#version 410\nout vec4 LFragment; void main() { LFragment = vec4(1.0, 1.0, 1.0, 1.0); }"
	//};
	std::string fragmentShaderSource = ReadFile("..\\..\\..\\src\\shaders\\DiffuseTextureShader.frag");
	const char* fragmentShaderSourceStr = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceStr, NULL);
	glCompileShader(fragmentShader);
	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
	printShaderLog(fragmentShader);
	if (fShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile fragment shader %d!\n", fragmentShader);
		return false;
	}
	glAttachShader(g_programID, fragmentShader);

	glLinkProgram(g_programID);
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(g_programID, GL_LINK_STATUS, &programSuccess);
	printProgramLog(g_programID);
	if (programSuccess != GL_TRUE)
	{
		printf("Error linking program %d!\n", g_programID);
		return false;
	}

	if (!CreateProgram2())
	{
		return false;
	}

	g_projectionViewModelMatrixID2 = glGetUniformLocation(g_programID2, "projectionViewModel");
	g_paletteID2 = glGetUniformLocation(g_programID2, "palette");

	g_projectionViewModelMatrixID = glGetUniformLocation(g_programID, "projectionViewModel");
	g_paletteID = glGetUniformLocation(g_programID, "palette");
	g_diffuseTextureLocation = glGetUniformLocation(g_programID, "diffuseTexture");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	CE::Skeleton* skeleton = CE::SkeletonManager::Get().GetSkeleton(g_fbxName);
	CE::Meshes* meshes = CE::MeshManager::Get().GetMeshes(g_fbxName, *skeleton);
	CE::Animations* animations = CE::AnimationManager::Get().GetAnimations(g_fbxName, *skeleton);

	for (int i = 0; i < meshes->size(); ++i)
	{
		CE::TextureManager::Get().GetTexture(meshes->at(i).m_diffuseMapName.c_str());
	}

	CE::AnimationOptimizer optimizer = CE::AnimationOptimizer(animations);
	optimizer.OptimizeAnimations();

	g_meshComponent = new CE::MeshComponent(meshes);
	g_animationComponent = new CE::AnimationComponent(skeleton, animations);

	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	// TODO: Why do I need to bind the buffer immediately after generating it?
	glGenBuffers(1, &g_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);

	glGenBuffers(1, &g_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);

	g_diffuseTextureUnit = 0;
	glActiveTexture(GL_TEXTURE0 + g_diffuseTextureUnit);
	glGenTextures(1, &g_diffuseTextureID);
	glBindTexture(GL_TEXTURE_2D, g_diffuseTextureID);

	// TODO: How much of this do I need to do here, versus every call?
	g_paletteTextureUnit = 1;
	glGenBuffers(1, &g_tbo);
	glBindBuffer(GL_TEXTURE_BUFFER, g_tbo);
	glActiveTexture(GL_TEXTURE0 + g_paletteTextureUnit);
	glGenTextures(1, &g_paletteGenTex);
	glBindTexture(GL_TEXTURE_BUFFER, g_paletteGenTex);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, g_tbo);

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

	g_fbxImporter = new CE::FBXImporter();
	g_stbiImporter = new CE::STBImageImporter();

	CE::MeshManager::Get().Initialize(g_fbxImporter);
	CE::AnimationManager::Get().Initialize(g_fbxImporter);
	CE::SkeletonManager::Get().Initialize(g_fbxImporter);
	CE::TextureManager::Get().Initialize(g_stbiImporter);

	if (!InitializeOpenGL())
	{
		printf("Unable to initialize OpenGL!\n");
		return false;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("GL_RENDERER: %s\n", renderer);
	printf("GL_VERSION: %s\n", version);
	// TODO: Doesn't work on my laptop's Intel HD Graphics 4000, which only supports up to OpenGL 4.0.

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
	CE::AnimationManager::Get().Destroy();
	CE::SkeletonManager::Get().Destroy();
	CE::TextureManager::Get().Destroy();

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (!Initialize())
	{
		printf("Failed to initialize.\n");
		getchar();
		return -1;
	}

	bool quit = false;
	SDL_Event event;

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
					//		break;
					//	case SDLK_DOWN:
					//		break;
					//	case SDLK_LEFT:
					//		break;
					//	case SDLK_RIGHT:
					//		break;
					//	default:
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

		g_animationComponent->Update(deltaTime);

		Render();
		SDL_GL_SwapWindow(g_window);
	}

	SDL_StopTextInput();

	Destroy();

	return 0;
}
