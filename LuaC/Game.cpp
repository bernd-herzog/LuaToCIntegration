#include "stdafx.h"
#include "Game.h"
#include "CppWrapper.h"
#include <assert.h>

MakeWrapper(DisplayWrapper, (), (), Game, void);
MakeWrapper(ReshapeWrapper, (int w, int h), (w, h), Game, void);
MakeWrapper(KeyboardWrapper, (unsigned char c, int p1, int p2), (c, p1, p2), Game, void);
MakeWrapper(SpecialWrapper, (int c, int p1, int p2), (c, p1, p2), Game, void);
MakeWrapper(MouseWrapper, (int button, int state, int x, int y), (button, state, x, y), Game, void);
MakeWrapper(MotionWrapper, (int x, int y), (x, y), Game, void);


Game::Game(void)
	:m_vertexShader(-1), m_fragmentShader(-1), m_angleX(0.0f), m_angleY(0.0f), m_oldX(0.0f), m_oldY(0.0f)
{
}



Game::~Game(void)
{
}

void Game::Scripting_OnInstallFragmentShader(Scripting*, EventArgs<char *> args)
{
	this->CreateFragmentShader(args.arg1);
}

void Game::Scripting_OnInstallVertexShader(Scripting*, EventArgs<char *> args)
{
	this->CreateVertexShader(args.arg1);
}


void Game::Init()
{
	//	wglCreateContext()

	int i = 0;
	glutInit(&i, NULL);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(800, 600);
	glutCreateWindow("LUA");
	GLenum err = glewInit();


	const char *venstr = (const char*)glGetString(GL_VENDOR);
	const char *verstr = (const char*)glGetString(GL_VERSION);

	glutDisplayFunc(GetWrapper(DisplayWrapper, &Game::glutDisplay));
	glutReshapeFunc(GetWrapper(ReshapeWrapper, &Game::glutReshape));
	glutKeyboardFunc(GetWrapper(KeyboardWrapper, &Game::glutKeyboard));
	glutSpecialFunc(GetWrapper(SpecialWrapper, &Game::glutSpecial));
	glutMouseFunc(GetWrapper(MouseWrapper, &Game::glutMouse));
	glutMotionFunc(GetWrapper(MotionWrapper, &Game::glutMotion));

	const GLchar *vertexSource =
		"#version 120\n"
		"varying vec3 normal;\n"
		"varying vec4 pos;\n"
		"void main(void) {\n"
		"	normal = gl_NormalMatrix * gl_Normal;\n"
		"	pos = gl_ModelViewMatrix * gl_Vertex;\n"
		"	gl_FrontColor = gl_Color; gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
		"}";

	const GLchar *fragmentSource =
		"#version 120\n"
		"varying vec3 normal;\n"
		"varying vec4 pos;\n"
		"void main(void){\n"
		"	vec3 l_norm = normalize(vec3((gl_LightSource[0].position) - pos));\n" 
		"	vec3 n_norm = normalize (normal);\n"
		"	float dot_n_l = dot (n_norm, l_norm);\n"
		"	float iDiff = max(0.0f, dot_n_l);\n"
		"	vec3 c_norm = normalize(-vec3(pos));\n"
		"	float iSpec = max(0.0f, pow(dot(2 * dot_n_l * n_norm - l_norm, c_norm), gl_FrontMaterial.shininess));\n"
		"	gl_FragColor = gl_FrontMaterial.emission + gl_Color * (iDiff * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse + iSpec * gl_FrontMaterial.specular * gl_LightSource[0].specular + gl_FrontMaterial.ambient * gl_LightSource[0].ambient);\n"
		"}";

	this->m_shaderProgram = glCreateProgram();

	this->CreateVertexShader(vertexSource);
	assert(glGetError() == GL_NO_ERROR);
	this->CreateFragmentShader(fragmentSource);
	assert(glGetError() == GL_NO_ERROR);

	m_scripting.InstallFragmentShader.attach<Game, &Game::Scripting_OnInstallFragmentShader>(this);
	m_scripting.InstallVertexShader.attach<Game, &Game::Scripting_OnInstallVertexShader>(this);

	m_scripting.Init();
	m_scripting.RunScripts();
}

bool Game::CreateVertexShader(const char *source)
{
	GLint shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader, 1, &source, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE){
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		this->m_scripting.PostLogEvent((char *)&errorLog[0]);
		//Provide the infolog in whatever manor you deem best.
		//Exit with failure.
		glDeleteShader(shader); //Don't leak the shader.
		return false;
	}

	if (m_vertexShader != -1)
	{
		glDetachShader(this->m_shaderProgram, this->m_vertexShader);
		glDeleteShader(this->m_vertexShader);
	}


	m_vertexShader = shader;

	glAttachShader(this->m_shaderProgram, this->m_vertexShader);
	glLinkProgram(this->m_shaderProgram);

	glGetProgramiv(this->m_shaderProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);


		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(this->m_shaderProgram, maxLength, &maxLength, &errorLog[0]);
		this->m_scripting.PostLogEvent((char *)&errorLog[0]);

		return false;
	}

	return true;
}

bool Game::CreateFragmentShader(const char *source)
{
	GLint shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &source, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE){
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		this->m_scripting.PostLogEvent((char *)&errorLog[0]);
		//Provide the infolog in whatever manor you deem best.
		//Exit with failure.
		glDeleteShader(shader); //Don't leak the shader.
		return false;
	}

	if (m_fragmentShader != -1)
	{
		glDetachShader(this->m_shaderProgram, this->m_fragmentShader);
		glDeleteShader(this->m_fragmentShader);
	}


	m_fragmentShader = shader;

	glAttachShader(this->m_shaderProgram, this->m_fragmentShader);
	glLinkProgram(this->m_shaderProgram);

	glGetProgramiv(this->m_shaderProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(this->m_shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(this->m_shaderProgram, maxLength, &maxLength, &errorLog[0]);
		this->m_scripting.PostLogEvent((char *)&errorLog[0]);

		return false;
	}

	return true;
}


void Game::MainLoop()
{
	glutMainLoop();
}


void Game::glutDisplay()
{

	//init gl for 3d
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(g_3dMatrix);
	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	float x = cosf(m_angleY) * sinf(m_angleX);
	float y = sinf(m_angleY);
	float z = cosf(m_angleY) * cosf(m_angleX);

	gluLookAt(
		x, y, z, //eye
		0.0f, 0.0f, 0.0f, //center
		0.0f, 1.0f, 0.0f);//up

	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

	glUseProgram(this->m_shaderProgram);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//light
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_FRONT, GL_SPECULAR, light_specular);
	glLightfv(GL_FRONT, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_FRONT, GL_AMBIENT, light_ambient);

	//material
	GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };

	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[] = { 50.0f };

	GLfloat mat_emmision[] = { 0.0, 0.0, 0.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emmision);


	//scene
	glutSolidTeapot(0.3);
	//	glutSolidCone(0.2, 0.2, 4, 4);

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);


	glUseProgram(0);


	//glBegin(GL_TRIANGLES);
	//{
	//	glVertex3f(0.0f, 0.0f, -2.0f);
	//	glVertex3f(1.0f, 0.0f, -2.0f);
	//	glVertex3f(0.0f, 1.0f, -2.0f);
	//}
	//glEnd();

	//init gl for HUD
	glDisable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(
		0.0f, 0.0f, 1.0f, //eye
		0.0f, 0.0f, 0.0f, //center
		0.0f, 1.0f, 0.0f);//up

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(g_HudMatrix);
	glMatrixMode(GL_MODELVIEW);


	m_scripting.RenderHUD();

	glutSwapBuffers();
	glutPostRedisplay();

}

void Game::glutReshape(int w, int h)
{
	this->m_windowHeight = h;
	this->m_windowWidth = w;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	//3dMatrix
	glLoadIdentity();
	gluPerspective(65.0, (float)w / h, 0.1f, 100.0f);

	glGetFloatv(GL_PROJECTION_MATRIX, g_3dMatrix);
	//hud matrix
	glLoadIdentity();
	glOrtho(0.0, w, 0.0, h, -1.0, 1.0); // {0,0} ist unten links

	glGetFloatv(GL_PROJECTION_MATRIX, g_HudMatrix);

	glMatrixMode(GL_MODELVIEW);


	SIZE s;
	s.cx = w;
	s.cy = h;
	m_scripting.SetUiSize(s);
}

void Game::glutKeyboard(unsigned char c, int p1, int p2)
{
	m_scripting.KeyboardEvent(c, p1, p2);

	/*
	if (c == 'f')
	{
	glutFullScreenToggle();
	}
	if (c == 'e')
	{
	exit(0);
	}*/
}

void Game::glutSpecial(int c, int p1, int p2)
{
	m_scripting.SpecialEvent(c, p1, p2);
}


void Game::glutMouse(int button, int state, int x, int y)
{
	bool handled = m_scripting.MouseEvent(button, state, x, this->m_windowHeight - y);

	if (!handled || button == GLUT_UP)
	{
		m_oldX = x;
		m_oldY = y;
		m_inMotion = state == GLUT_DOWN;
	}
}

void Game::glutMotion(int x, int y)
{
	if (m_inMotion){
		if (m_inMotion) {
			int dx = m_oldX - x;
			int dy = m_oldY - y;

			m_angleX += (float)dx / 360 * 3.1415f;
			m_angleY -= (float)dy / 360 * 3.1415f;

			if (m_angleY > 3.1415f/2)
				m_angleY = 3.1415f/2;
			if (m_angleY < -3.1415f/2)
				m_angleY = -3.1415f/2;

		}
		m_oldX = x;
		m_oldY = y;
	}
}
