// Main for Darwin

#ifdef __APPLE__

#include <DarwinPCH.h>
#include <Camera.h>

#define BUFFER_OFFSET(offset) ((void*)(offset))
#define MEMBER_OFFSET(s,m) ((char*)NULL + (offsetof(s,m)))

int g_iWindowWidth = 800;
int g_iWindowHeight = 600;
int g_iWindowHandle = 0;

int g_W, g_A, g_S, g_D, g_Q, g_E;
bool g_bShift = false;

glm::ivec2 g_MousePos;

glm::quat g_Rotation;

std::clock_t g_PreviousTicks;
std::clock_t g_CurrentTicks;

Camera g_Camera;
glm::vec3 g_InitialCameraPosition;
glm::quat g_InitialCameraRotation;

struct VertexXYZColor
{
	glm::vec3 m_Pos;
	glm::vec3 m_Color;
};

// Define the 8 vertices of a unit cube
VertexXYZColor g_Vertices[8] = {
	{ glm::vec3(1,  1,  1), glm::vec3(1, 1, 1) }, // 0
	{ glm::vec3(-1,  1,  1), glm::vec3(0, 1, 1) }, // 1
	{ glm::vec3(-1, -1,  1), glm::vec3(0, 0, 1) }, // 2
	{ glm::vec3(1, -1,  1), glm::vec3(1, 0, 1) }, // 3
	{ glm::vec3(1, -1, -1), glm::vec3(1, 0, 0) }, // 4
	{ glm::vec3(-1, -1, -1), glm::vec3(0, 0, 0) }, // 5
	{ glm::vec3(-1,  1, -1), glm::vec3(0, 1, 0) }, // 6
	{ glm::vec3(1,  1, -1), glm::vec3(1, 1, 0) }, // 7
};

// Define the vertex indices for the cube.
// Each set of 6 vertices represents a set of triangles in 
// counter-clockwise winding order.
GLuint g_Indices[36] = {
	0, 1, 2, 2, 3, 0,           // Front face
	7, 4, 5, 5, 6, 7,           // Back face
	6, 5, 2, 2, 1, 6,           // Left face
	7, 0, 3, 3, 4, 7,           // Right face
	7, 6, 1, 1, 0, 7,           // Top face
	3, 2, 5, 5, 4, 3            // Bottom face
};

// Vertex array object for the cube.
GLuint g_vaoCube = 0;
GLuint g_ShaderProgram = 0;
// Model, View, Projection matrix uniform variable in shader program.
GLint g_uniformMVP = -1;

void IdleGL();
void DisplayGL();
void KeyboardGL(unsigned char c, int x, int y);
void KeyboardUpGL(unsigned char c, int x, int y);
void SpecialGL(int key, int x, int y);
void SpecialUpGL(int key, int x, int y);
void MouseGL(int button, int state, int x, int y);
void MotionGL(int x, int y);
void ReshapeGL(int w, int h);

/**
* Initialize the OpenGL context and create a render window.
*/
void InitGL(int argc, char* argv[])
{
	std::cout << "Initialize OpenGL..." << std::endl;

	glutInit(&argc, argv);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	int iScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int iScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

	// Create an OpenGL 3.3 core forward compatible context.
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitWindowPosition((iScreenWidth - g_iWindowWidth) / 2, (iScreenHeight - g_iWindowHeight) / 2);
	glutInitWindowSize(g_iWindowWidth, g_iWindowHeight);

	g_iWindowHandle = glutCreateWindow("OpenGL Template");

	// Register GLUT callbacks.
	glutIdleFunc(IdleGL);
	glutDisplayFunc(DisplayGL);
	glutKeyboardFunc(KeyboardGL);
	glutKeyboardUpFunc(KeyboardUpGL);
	glutSpecialFunc(SpecialGL);
	glutSpecialUpFunc(SpecialUpGL);
	glutMouseFunc(MouseGL);
	glutMotionFunc(MotionGL);
	glutReshapeFunc(ReshapeGL);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	std::cout << "Initialize OpenGL Success!" << std::endl;
}

void InitGLEW()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "There was a problem initializing GLEW. Exiting..." << std::endl;
		exit(-1);
	}

	// Check for 3.3 support.
	// I've specified that a 3.3 forward-compatible context should be created.
	// so this parameter check should always pass if our context creation passed.
	// If we need access to deprecated features of OpenGL, we should check
	// the state of the GL_ARB_compatibility extension.
	if (!GLEW_VERSION_3_3)
	{
		std::cerr << "OpenGL 3.3 required version support not present." << std::endl;
		exit(-1);
	}
}

// Loads a shader and returns the compiled shader object.
// If the shader source file could not be opened or compiling the 
// shader fails, then this function returns 0.
GLuint LoadShader(GLenum shaderType, const std::string& shaderFile)
{
	std::ifstream ifs;

	// Load the shader source file.
	ifs.open(shaderFile);

	if (!ifs)
	{
		std::cerr << "Can not open shader file: \"" << shaderFile << "\"" << std::endl;
		return 0;
	}

	std::string source(std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()));
	ifs.close();

	// Create a shader object.
	GLuint shader = glCreateShader(shaderType);

	// Load the shader source for each shader object.
	const GLchar* sources[] = { source.c_str() };
	glShaderSource(shader, 1, sources, NULL);
	glCompileShader(shader);

	// Check for errors
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog(shader, logLength, NULL, infoLog);

#ifdef _WIN32
		OutputDebugString(infoLog);
#else
		std::cerr << infoLog << std::endl;
#endif
		delete infoLog;
		return 0;
	}
	return shader;
}

// Create a shader program from a set of compiled shader objects.
GLuint CreateShaderProgram(std::vector<GLuint> shaders)
{
	// Create a shader program.
	GLuint program = glCreateProgram();

	// Attach the appropriate shader objects.
	for (GLuint shader : shaders)
	{
		glAttachShader(program, shader);
	}

	// Link the program
	glLinkProgram(program);

	// Check the link status.
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];

		glGetProgramInfoLog(program, logLength, NULL, infoLog);


		std::cerr << infoLog << std::endl;


		delete infoLog;
		return 0;
	}
	return program;
}


int main(int argc, char* argv[])
{
	g_PreviousTicks = std::clock();
	g_A = g_W = g_S = g_D = g_Q = g_E = 0;

	g_InitialCameraPosition = glm::vec3(0, 0, 10);
	g_Camera.SetPosition(g_InitialCameraPosition);
	g_Camera.SetRotation(g_InitialCameraRotation);

	InitGL(argc, argv);
	InitGLEW();

	// Load some shaders.
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, "../../../CompositeEngine/data/shaders/simpleShader.vert");
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "../../../CompositeEngine/data/shaders/simpleShader.frag");

	std::vector<GLuint> shaders;
	shaders.push_back(vertexShader);
	shaders.push_back(fragmentShader);

	// Create the shader program.
	g_ShaderProgram = CreateShaderProgram(shaders);
	assert(g_ShaderProgram != 0);

	GLint positionAtribID = glGetAttribLocation(g_ShaderProgram, "in_position");
	GLint colorAtribID = glGetAttribLocation(g_ShaderProgram, "in_color");
	g_uniformMVP = glGetUniformLocation(g_ShaderProgram, "MVP");

	// Create a VAO for the cube.
	glGenVertexArrays(1, &g_vaoCube);
	glBindVertexArray(g_vaoCube);

	GLuint vertexBuffer, indexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_Vertices), g_Vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_Indices), g_Indices, GL_STATIC_DRAW);

	glVertexAttribPointer(positionAtribID, 3, GL_FLOAT, false, sizeof(VertexXYZColor), MEMBER_OFFSET(VertexXYZColor, m_Pos));
	glEnableVertexAttribArray(positionAtribID);

	glVertexAttribPointer(colorAtribID, 3, GL_FLOAT, false, sizeof(VertexXYZColor), MEMBER_OFFSET(VertexXYZColor, m_Color));
	glEnableVertexAttribArray(colorAtribID);

	// Make sure we disable and unbind everything to prevent rendering issues later.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(positionAtribID);
	glDisableVertexAttribArray(colorAtribID);


	glutMainLoop();
}

void ReshapeGL(int w, int h)
{
	if (h == 0)
	{
		h = 1;
	}

	g_iWindowWidth = w;
	g_iWindowHeight = h;

	g_Camera.SetViewport(0, 0, w, h);
	g_Camera.SetProjectionRH(60.0f, w / (float)h, 0.1f, 100.0f);

	glutPostRedisplay();
}

void DisplayGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(g_vaoCube);
	glUseProgram(g_ShaderProgram);

	glm::mat4 mvp = g_Camera.GetProjectionMatrix() * g_Camera.GetViewMatrix() * glm::toMat4(g_Rotation);
	glUniformMatrix4fv(g_uniformMVP, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	glUseProgram(0);
	glBindVertexArray(0);

	glutSwapBuffers();
}

void IdleGL()
{
	g_CurrentTicks = std::clock();
	float deltaTicks = (float)(g_CurrentTicks - g_PreviousTicks);
	g_PreviousTicks = g_CurrentTicks;

	float fDeltaTime = deltaTicks / (float)CLOCKS_PER_SEC;

	float cameraSpeed = 1.0f;
	if (g_bShift)
	{
		cameraSpeed = 5.0f;
	}

	g_Camera.Translate(glm::vec3(g_D - g_A, g_Q - g_E, g_S - g_W) * cameraSpeed * fDeltaTime);

	glutPostRedisplay();
}

void KeyboardGL(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'w':
	case 'W':
		g_W = 1;
		break;
	case 'a':
	case 'A':
		g_A = 1;
		break;
	case 's':
	case 'S':
		g_S = 1;
		break;
	case 'd':
	case 'D':
		g_D = 1;
		break;
	case 'q':
	case 'Q':
		g_Q = 1;
		break;
	case 'e':
	case 'E':
		g_E = 1;
		break;
	case 'r':
	case 'R':
		g_Camera.SetPosition(g_InitialCameraPosition);
		g_Camera.SetRotation(g_InitialCameraRotation);
		g_Rotation = glm::quat();
		break;
	case 27:
		glutLeaveMainLoop();
		break;
	}
}

void KeyboardUpGL(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'w':
	case 'W':
		g_W = 0;
		break;
	case 'a':
	case 'A':
		g_A = 0;
		break;
	case 's':
	case 'S':
		g_S = 0;
		break;
	case 'd':
	case 'D':
		g_D = 0;
		break;
	case 'q':
	case 'Q':
		g_Q = 0;
		break;
	case 'e':
	case 'E':
		g_E = 0;
		break;

	default:
		break;
	}
}

void SpecialGL(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_SHIFT_L:
	case GLUT_KEY_SHIFT_R:
	{
		g_bShift = true;
	}
	break;
	}
}

void SpecialUpGL(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_SHIFT_L:
	case GLUT_KEY_SHIFT_R:
	{
		g_bShift = false;
	}
	break;
	}
}

void MouseGL(int button, int state, int x, int y)
{
	g_MousePos = glm::ivec2(x, y);
}

void MotionGL(int x, int y)
{
	glm::ivec2 mousePos = glm::ivec2(x, y);
	glm::vec2 delta = glm::vec2(mousePos - g_MousePos);
	g_MousePos = mousePos;

	std::cout << "dX: " << delta.x << " dy: " << delta.y << std::endl;

	glm::quat rotX = glm::angleAxis<float>(glm::radians(delta.y) * 0.5f, glm::vec3(1, 0, 0));
	glm::quat rotY = glm::angleAxis<float>(glm::radians(delta.x) * 0.5f, glm::vec3(0, 1, 0));

	//g_Camera.Rotate( rotX * rotY );
	g_Rotation = (rotX * rotY) * g_Rotation;
}

#endif
